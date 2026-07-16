#include <controller/UserController.hpp>
#include <wfrest/BluePrint.h>
#include <wfrest/HttpMsg.h>
#include <workflow/Workflow.h>
#include <services/UserService.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std::placeholders;
namespace controller
{
    UserController::UserController(services::UserService &userService)
        : userService_(userService)
    {
    }
    UserController::~UserController() = default;
    void UserController::registerRoutes(wfrest::BluePrint &bp)
    {
        bp.POST("/register", std::bind(&UserController::registerHandler,this,_1,_2,_3));
        bp.POST("/login", std::bind(&UserController::loginHandler,this,_1,_2,_3));
    }

    // 注册用户
    void UserController::registerHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw)
    {
        // 解析请求体
        std::string req_body = req->body();
        // 设置上下文
        models::UserContext *user = new models::UserContext();
        sw->set_context(user);
        // 解析JSON
        try
        {
            json req_json = json::parse(req_body);
            std::string  username = req_json["username"].get<std::string>();
            std::string password = req_json["password"].get<std::string>();
            std::string confirm = req_json["confirm"].get<std::string>();

            json resp_json;
            // 注册用户
            SubTask *task = userService_.register_user(username, password,[username,resp_json,resp,sw](bool ok, std::string msg) mutable
            {
                if(ok){
                    // 获取上下文
                    models::UserContext *user_ctx = static_cast<models::UserContext*>(sw->get_context());

                    resp_json["status"] = "success";
                    resp_json["message"] = "注册成功";
                    resp_json["data"]["userId"] = user_ctx->user_id;
                    resp_json["data"]["username"] = username;
                    resp->Json(resp_json.dump());
                }
                else{
                    resp_json["status"] = "error";
                    resp_json["message"] = msg;
                    resp->Json(resp_json.dump());
                }
            });
            sw->push_back(task);
        }
        catch(const std::exception& e)
        {
            resp->set_status_code("400");
            resp->String("Invalid JSON body");
            return;
        }
    }
    // 登录用户
    void UserController::loginHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw)
    {
        // 解析请求体
        std::string req_body = req->body();
        // 设置上下文
        models::UserContext *user_ctx = new models::UserContext();
        sw->set_context(user_ctx);
        // 解析JSON
        try
        {
            json req_json = json::parse(req_body);
            std::string  username = req_json["username"].get<std::string>();
            std::string password = req_json["password"].get<std::string>();

            json resp_json;
            // 登录用户
            SubTask *task = userService_.login_user(username, password,[username,resp_json,resp,sw](bool ok, std::string msg,std::string token) mutable
            {
                if(ok){
                    // 获取上下文
                    models::UserContext *user_ctx = static_cast<models::UserContext*>(sw->get_context());
                    
                    resp_json["status"] = "success";
                    resp_json["message"] = "登录成功";
                    resp_json["data"]["token_type"] = "Bearer";
                    resp_json["data"]["accessToken"] = token;
                    resp_json["data"]["user"]["userId"] = user_ctx->user_id;
                    resp_json["data"]["user"]["user_name"] = user_ctx->user_name;
                    resp->Json(resp_json.dump());
                }
                else{
                    resp_json["status"] = "error";
                    resp_json["message"] = msg;
                    resp->Json(resp_json.dump());
                }
            });
            sw->push_back(task);
        }
        catch(const std::exception& e)
        {
            resp->set_status_code("400");
            resp->String("Invalid JSON body");
            return;
        }
    }
}
