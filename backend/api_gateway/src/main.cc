#include <wfrest/HttpServer.h>
#include <wfrest/HttpMsg.h>
#include <nlohmann/json.hpp>
#include <workflow/WFFacilities.h>
#include <wfrest/BluePrint.h>
#include <signal.h>
#include "user.srpc.h"
#include "Config.hpp"
#include "Logger.hpp"
#include "JWT.hpp"

WFFacilities::WaitGroup waitGroup(1);

void sig_handler(int signo)
{
	waitGroup.done();
}

using json = nlohmann::json;
int main(int argc, char const *argv[])
{
    signal(SIGINT,sig_handler);
    // 初始化配置
    utils::Config config("conf/app.json");

    // 初始化日志
    utils::Logger::init("log/api_gateway.log","info",0,0);
    LOG_INFO("api_gateway starting...");

    // 启动HTTP服务器
    wfrest::HttpServer server;

    // 实例化rpc客户端
    cloud_disk::UserService::SRPCClient user_client("127.0.0.1",8081);


    // 注册静态资源
    server.Static("/","../frontend/www/index.html");
    server.Static("/static","../frontend/www/static");
    // 认证蓝图
    wfrest::BluePrint auth_bp;
    auth_bp.POST("/auth/login",[&user_client](const wfrest::HttpReq *req,wfrest::HttpResp *resp,SeriesWork *sw){
        try
        {
            // 解析json
            json j = json::parse(req->body());
            // 从json中提取用户名和密码
            cloud_disk::LoginRequest rpc_req;
            rpc_req.set_username(j["username"].get<std::string>());
            rpc_req.set_password(j["password"].get<std::string>());
            
            // 创建rpc任务
            auto *task = user_client.create_Login_task([resp](const cloud_disk::LoginResponse *rpc_resp,const srpc::RPCContext *ctx){
                if(ctx->success()){
                    json resp_json;
                    resp_json["status"] = rpc_resp->status();
                    resp_json["message"] = rpc_resp->message();

                    if(rpc_resp->status() == "success"){
                        resp_json["data"]["accessToken"] = rpc_resp->access_token();
                        resp_json["data"]["user"]["userId"] = rpc_resp->userid();
                        resp_json["data"]["user"]["username"] = rpc_resp->username();
                        resp_json["data"]["token_type"] = rpc_resp->token_type();
                        LOG_INFO("login success, userId: {}, username: {}",rpc_resp->userid(),rpc_resp->username());
                    }else{
                        resp->set_status_code("401");
                        LOG_INFO("login failed, message: {}",rpc_resp->message());
                    }
                    resp->Json(resp_json.dump());
                }else
                {
                    LOG_WARN("rpc login failed: {}",ctx->get_errmsg());
                    resp->set_status_code("502");
                    resp->String("Internal Server Error");
                }
            });
            task->serialize_input(&rpc_req);
            sw->push_back(task);
        }
        catch(const std::exception& e)
        {
            resp->set_status_code("500");
            resp->String("Internal Server Error");
        }
    });
    // 注册用户
    auth_bp.POST("/auth/register",[&user_client](const wfrest::HttpReq *req,wfrest::HttpResp *resp,SeriesWork *sw){
        try
        {
            // 解析json
            json j = json::parse(req->body());
            // 从json中提取用户名和密码
            cloud_disk::RegisterRequest rpc_req;
            rpc_req.set_username(j["username"].get<std::string>());
            rpc_req.set_password(j["password"].get<std::string>());
            
            // 创建rpc任务
            auto *task = user_client.create_Register_task([resp](const cloud_disk::RegisterResponse *rpc_resp,const srpc::RPCContext *ctx){
                if(ctx->success()){
                    json resp_json;
                    resp_json["status"] = rpc_resp->status();
                    

                    if(rpc_resp->status() == "success"){
                        resp_json["data"]["userId"] = rpc_resp->userid();
                        resp_json["data"]["username"] = rpc_resp->username();
                        resp_json["message"] = "注册成功";
                        LOG_INFO("register success, userId: {}, username: {}",rpc_resp->userid(),rpc_resp->username());
                    }else{
                        resp->set_status_code("401");
                        resp_json["message"] = "注册失败";
                        LOG_INFO("register failed, message: {}",rpc_resp->message());
                    }
                    resp->Json(resp_json.dump());
                }else
                {
                    LOG_WARN("rpc register failed: {}",ctx->get_errmsg());
                    resp->set_status_code("502");
                    resp->String("Internal Server Error");
                }
            });
            task->serialize_input(&rpc_req);
            sw->push_back(task);
        }
        catch(const std::exception& e)
        {
            resp->set_status_code("500");
            resp->String("Internal Server Error");
        }
    });

    // 获取用户信息
    auth_bp.GET("/user/me",[&user_client](const wfrest::HttpReq *req,wfrest::HttpResp *resp,SeriesWork *sw)
    {
        // 解析获取token
        std::string auth_header = req->header("Authorization");
        std::string token = "";
        json resp_json;

        if (auth_header.length() > 7 && auth_header.substr(0, 7) == "Bearer ") {
            token = auth_header.substr(7); // 截掉前面的 "Bearer "，剩下的就是纯 Token
            LOG_INFO("token: {}",token);
        }
        if (token.empty()) {
            resp->set_status_code("401"); // 401 Unauthorized
            resp_json["status"] = "error";
            resp_json["message"] = "未授权，请提供有效的 Token";
            resp->Json(resp_json.dump());
            return;
        }
        utils::JWT jwt = utils::JWT("secret");
        int user_id = 0;
        int role = 0;
        if(jwt.verifyJWT(token,user_id,role)){
            cloud_disk::GetUserRequest rpc_req;
            rpc_req.set_userid(user_id);
            auto *task = user_client.create_GetUser_task([resp_json,resp](const cloud_disk::GetUserResponse *rpc_resp,const srpc::RPCContext *ctx) mutable
            {
                if(ctx->success()){
                    resp_json["status"] = rpc_resp->status();
                    if(rpc_resp->status() == "success"){
                        resp_json["message"] = "获取个人信息成功";
                        resp_json["data"]["userId"] = rpc_resp->userid();
                        resp_json["data"]["username"] = rpc_resp->username();
                        resp_json["data"]["createdAt"] = rpc_resp->createdat();
                    }else{
                        resp->set_status_code("401");
                        resp_json["message"] = rpc_resp->message();
                        LOG_INFO("get user info failed, message: {}",rpc_resp->message());
                    }
                    resp->Json(resp_json.dump());
                }else
                {
                    LOG_WARN("rpc get user info failed: {}",ctx->get_errmsg());
                    resp->set_status_code("502");
                    resp_json["status"] = "error";
                    resp_json["message"] = "Internal Server Error";
                    resp->Json(resp_json.dump());
                }
            });
            task->serialize_input(&rpc_req);
            sw->push_back(task);
        }else
        {
            resp->set_status_code("401"); // 401 Unauthorized
            resp_json["status"] = "error";
            resp_json["message"] = "无效的 Token";
            resp->Json(resp_json.dump());
            return;
        }
    });

    server.register_blueprint(auth_bp,"/api/v1");

    
    unsigned short port = 8080;
    if(server.start(port) == 0){
        LOG_INFO("api_gateway start in port {}",port);
        waitGroup.wait();
        LOG_INFO("api_gateway stop success");
        server.stop();
        google::protobuf::ShutdownProtobufLibrary();
    }else
    {
        LOG_ERROR("api_gateway start failed");
        return -1;
    }
    return 0;
}


