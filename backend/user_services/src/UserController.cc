
#include <nlohmann/json.hpp>
#include "UserService.hpp"
#include "UserController.hpp"
#include "JWT.hpp"
#include "user.pb.h"

using json = nlohmann::json;
using namespace std::placeholders;
namespace controller
{
    UserController::UserController(services::UserService &userService)
        : userService_(userService)
    {
    }
    UserController::~UserController() = default;

    // 用户登录
    void UserController::Login(cloud_disk::LoginRequest *req, cloud_disk::LoginResponse *resp,srpc::RPCContext *ctx)
    {
        std::string username = req->username();
        std::string password = req->password();

        // 创建设置上下文
        models::UserContext *user = new models::UserContext();
        ctx->get_series()->set_context(user);

        SubTask *task = userService_.login_user(username, password, [username,resp,ctx](bool ok, std::string msg, std::string token) mutable
        {
            if(ok){
                // 获取上下文
                models::UserContext *user_ctx = static_cast<models::UserContext*>(ctx->get_series()->get_context());
                resp->set_status("success");
                resp->set_message("登录成功");
                resp->set_access_token(token);
                resp->set_userid(user_ctx->user_id);
                resp->set_username(username);
                resp->set_token_type("Bearer");
            }
            else{
                resp->set_status("error");
                resp->set_message(msg);
            }
        });
        ctx->get_series()->push_back(task);
    }

    // 用户注册
    void UserController::Register(cloud_disk::RegisterRequest *req, cloud_disk::RegisterResponse *resp,srpc::RPCContext *ctx)
    {
        // 解析请求体
        std::string username = req->username();
        std::string password = req->password();
        // 设置上下文
        models::UserContext *user = new models::UserContext();
        ctx->get_series()->set_context(user);

        SubTask *task = userService_.register_user(username, password, [username,resp,ctx](bool ok, std::string msg) mutable
        {
            if(ok){
                // 获取上下文
                models::UserContext *user_ctx = static_cast<models::UserContext*>(ctx->get_series()->get_context());
                resp->set_status("success");
                resp->set_message(msg);
                resp->set_userid(user_ctx->user_id);
                resp->set_username(username);
            }
            else{
                resp->set_status("error");
                resp->set_message(msg);
            }
        });
        ctx->get_series()->push_back(task);
    }
    // 获取用户信息
    void UserController::GetUser(cloud_disk::GetUserRequest *req, cloud_disk::GetUserResponse *resp,srpc::RPCContext *ctx)
    {
        // 获取id
        int user_id = req->userid();

        // 创建任务
        SubTask *task = userService_.get_user_info(user_id,[resp,ctx](bool ok,std::string msg,models::SysUser userInfo){
            if(ok){
                resp->set_status("success");
                resp->set_message("获取用户信息成功");
                resp->set_userid(userInfo.id);
                resp->set_username(userInfo.user_name);
                resp->set_createdat(userInfo.create_time);
            }
            else{
                resp->set_status("error");
                resp->set_message(msg);
            }
        });
        ctx->get_series()->push_back(task);
    }

    // // 注册用户
    // void UserController::registerHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw)
    // {
    //     // 解析请求体
    //     std::string req_body = req->body();
    //     // 设置上下文
    //     models::UserContext *user = new models::UserContext();
    //     sw->set_context(user);
    //     // 解析JSON
    //     try
    //     {
    //         json req_json = json::parse(req_body);
    //         std::string  username = req_json["username"].get<std::string>();
    //         std::string password = req_json["password"].get<std::string>();
    //         std::string confirm = req_json["confirm"].get<std::string>();

    //         json resp_json;
    //         // 注册用户
    //         SubTask *task = userService_.register_user(username, password,[username,resp_json,resp,sw](bool ok, std::string msg) mutable
    //         {
    //             if(ok){
    //                 // 获取上下文
    //                 models::UserContext *user_ctx = static_cast<models::UserContext*>(sw->get_context());

    //                 resp_json["status"] = "success";
    //                 resp_json["message"] = "注册成功";
    //                 resp_json["data"]["userId"] = user_ctx->user_id;
    //                 resp_json["data"]["username"] = username;
    //                 resp->Json(resp_json.dump());
    //             }
    //             else{
    //                 resp_json["status"] = "error";
    //                 resp_json["message"] = msg;
    //                 resp->Json(resp_json.dump());
    //             }
    //         });
    //         sw->push_back(task);
    //     }
    //     catch(const std::exception& e)
    //     {
    //         resp->set_status_code("400");
    //         resp->String("Invalid JSON body");
    //         return;
    //     }
    // }
    // // 登录用户
    // void UserController::loginHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw)
    // {
    //     // 解析请求体
    //     std::string req_body = req->body();
    //     // 设置上下文
    //     models::UserContext *user_ctx = new models::UserContext();
    //     sw->set_context(user_ctx);
    //     // 解析JSON
    //     try
    //     {
    //         json req_json = json::parse(req_body);
    //         std::string  username = req_json["username"].get<std::string>();
    //         std::string password = req_json["password"].get<std::string>();

    //         json resp_json;
    //         // 登录用户
    //         SubTask *task = userService_.login_user(username, password,[username,resp_json,resp,sw](bool ok, std::string msg,std::string token) mutable
    //         {
    //             if(ok){
    //                 // 获取上下文
    //                 models::UserContext *user_ctx = static_cast<models::UserContext*>(sw->get_context());
                    
    //                 resp_json["status"] = "success";
    //                 resp_json["message"] = "登录成功";
    //                 resp_json["data"]["token_type"] = "Bearer";
    //                 resp_json["data"]["accessToken"] = token;
    //                 resp_json["data"]["user"]["userId"] = user_ctx->user_id;
    //                 resp_json["data"]["user"]["user_name"] = user_ctx->user_name;
    //                 resp->Json(resp_json.dump());
    //             }
    //             else{
    //                 resp->set_status_code("401");
    //                 resp_json["status"] = "error";
    //                 resp_json["message"] = msg;
    //                 resp->Json(resp_json.dump());
    //             }
    //         });
    //         sw->push_back(task);
    //     }
    //     catch(const std::exception& e)
    //     {
    //         resp->set_status_code("400");
    //         resp->String("Invalid JSON body");
    //         return;
    //     }
    // }


    // // 获取当前用户信息
    // void UserController::getUserHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw)
    // {
    //     // 解析获取token
    //     std::string auth_header = req->header("Authorization");
    //     std::string token = "";
    //     json resp_json;

    //     if (auth_header.length() > 7 && auth_header.substr(0, 7) == "Bearer ") {
    //         token = auth_header.substr(7); // 截掉前面的 "Bearer "，剩下的就是纯 Token
    //         std::cout << "token: " << token << std::endl;
    //     }
    //     if (token.empty()) {
    //         resp->set_status_code("401"); // 401 Unauthorized
    //         resp_json["status"] = "error";
    //         resp_json["message"] = "未授权，请提供有效的 Token";
    //         resp->Json(resp_json.dump());
    //         return;
    //     }
    //     utils::JWT jwt = utils::JWT("secret");
    //     int user_id = 0;
    //     int role = 0;
    //     if(jwt.verifyJWT(token,user_id,role)){
    //         resp_json["status"] = "success";
    //         resp_json["message"] = "获取用户信息成功";
    //         resp_json["data"]["userId"] = user_id;
    //         WFMySQLTask *task = userService_.get_user_info(user_id,[user_id,resp_json,resp,sw](bool ok, std::string msg, models::SysUser user) mutable
    //         {
    //             if(ok){
    //                 resp_json["data"]["userId"] = user.id;
    //                 resp_json["data"]["createTime"] = user.create_time;
    //                 resp->Json(resp_json.dump());
    //             }
    //             else{
    //                 resp_json["status"] = "error";
    //                 resp_json["message"] = msg;
    //                 resp->Json(resp_json.dump());
    //             }
    //         });
    //         sw->push_back(task);
    //     }else
    //     {
    //         resp->set_status_code("401"); // 401 Unauthorized
    //         resp_json["status"] = "error";
    //         resp_json["message"] = "无效的 Token";
    //         resp->Json(resp_json.dump());
    //         return;
    //     }
    // }
}
