
#include <controller/TlbFileController.hpp>
#include <nlohmann/json.hpp>
#include "JWT.hpp"

namespace controller
{
    using json = nlohmann::json;
    using namespace std::placeholders;
    // 构造
    TlbFileController::TlbFileController(services::TlbFileService &tlbFileService)
    : tlbFileService_(tlbFileService)
    {
    }
    // 注册路由
    void TlbFileController::registerRoutes(wfrest::BluePrint &bp)
    {
        bp.POST("/files",std::bind(&TlbFileController::uploadHandler,this,_1,_2,_3));
        bp.GET("/file/{id}",std::bind(&TlbFileController::downloadHandler,this,_1,_2,_3));
        bp.GET("/files/",std::bind(&TlbFileController::listAllHandler,this,_1,_2,_3));

    }
    // 析构
    TlbFileController::~TlbFileController()=default;
    // 上传文件
    void TlbFileController::uploadHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw)
    {
        // 解析获取token
        std::string content_type = req->header("Content-Type");
        std::cout << content_type << std::endl;
        std::string auth_header = req->header("Authorization");
        std::string token = "";
        json resp_json;
        if (auth_header.length() > 7 && auth_header.substr(0, 7) == "Bearer ") {
            token = auth_header.substr(7); // 截掉前面的 "Bearer "，剩下的就是纯 Token
        }
        if (token.empty()) {
            resp->set_status_code("401"); // 401 Unauthorized
            resp_json["status"] = "error";
            resp_json["message"] = "未授权，请提供有效的 Token";
            resp->Json(resp_json.dump());
            return;
        }
        utils::JWT jwt = utils::JWT("secret");
        int uid = 0;
        int role = 0;

        if(jwt.verifyJWT(token,uid,role)){  
            // 设置上下文
            models::TlbFileContext *file_ctx = new models::TlbFileContext();
            sw->set_context(file_ctx);
            std::string base_url = "/home/coisini/wangdao/code/cloud-disk/backend/upload/";
            const wfrest::Form &form = req->form();
            for (auto &it: form)
            {
                auto &name = it.first;
                auto &file_info = it.second;
                std::string filename = file_info.first;
                std::string physical_path = base_url + filename;
                SubTask *task = tlbFileService_.upload(uid,physical_path,file_info.second,0,[resp,sw,filename,resp_json](bool ok,std::string msg)mutable {
                    if(ok){
                        // 获取上下文
                        models::TlbFileContext *file_ctx = static_cast<models::TlbFileContext *>(sw->get_context());
                        resp_json["status"] = "success";
                        resp_json["message"] = "上传成功";
                        resp_json["data"]["fileId"] = file_ctx->id;
                        resp_json["data"]["filename"] = filename;
                        resp->Json(resp_json.dump());
                    }else{
                        resp->set_status_code("500");
                        resp_json["status"] = "error";
                        resp_json["message"] = msg;
                        resp->Json(resp_json.dump());
                    }
                });
                sw->push_back(task);
            }

        }else
        {
            resp->set_status_code("401"); // 401 Unauthorized
            resp_json["status"] = "error";
            resp_json["message"] = "无效的 Token";
            resp->Json(resp_json.dump());
            return;
        }
    }
    
    // 下载文件
    void TlbFileController::downloadHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw){
        // 设置上下文
        models::TlbFileContext *file_ctx = new models::TlbFileContext();
        sw->set_context(file_ctx);

        // 解析获取token
        std::string content_type = req->header("Content-Type");
        std::string auth_header = req->header("Authorization");
        std::string token = "";
        json resp_json;
        if (auth_header.length() > 7 && auth_header.substr(0, 7) == "Bearer ") {
            token = auth_header.substr(7); // 截掉前面的 "Bearer "，剩下的就是纯 Token
        }
        if (token.empty()) {
            resp->set_status_code("401"); // 401 Unauthorized
            resp_json["status"] = "error";
            resp_json["message"] = "未授权，请提供有效的 Token";
            resp->Json(resp_json.dump());
            return;
        }
        utils::JWT jwt = utils::JWT("secret");
        int uid = 0;
        int role = 0;
        if(jwt.verifyJWT(token,uid,role)){
            // 获取文件id
            int file_id = 0;
            std::string file_id_str = req->param("id");
            if(!file_id_str.empty()){
                file_id = std::stoi(file_id_str);
            }
            // 下载文件
            SubTask *task = tlbFileService_.download(file_id,[resp,sw,resp_json,file_ctx](bool ok,std::string msg, std::shared_ptr<std::string> content)mutable {
                if(ok){
                    // 设置响应头
                    resp->add_header_pair("Content-Disposition", "attachment; filename=" + file_ctx->filename);
                    resp->String(*content);
                }else{
                    resp->set_status_code("400");
                    resp_json["status"] = "error";
                    resp_json["message"] = msg;
                    resp->Json(resp_json.dump());
                }
            });
            sw->push_back(task);
        }else
        {
            resp->set_status_code("401"); // 401 Unauthorized
            resp_json["status"] = "error";
            resp_json["message"] = "无效的 Token";
            resp->Json(resp_json.dump());
            return;
        }
        
    }

    // 列出所有文件
    void TlbFileController::listAllHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw)
    {
        // 解析获取token
        std::string content_type = req->header("Content-Type");
        std::cout << content_type << std::endl;
        std::string auth_header = req->header("Authorization");
        std::string token = "";
        json resp_json;
        if (auth_header.length() > 7 && auth_header.substr(0, 7) == "Bearer ") {
            token = auth_header.substr(7); // 截掉前面的 "Bearer "，剩下的就是纯 Token
        }
        if (token.empty()) {
            resp->set_status_code("401"); // 401 Unauthorized
            resp_json["status"] = "error";
            resp_json["message"] = "未授权，请提供有效的 Token";
            resp->Json(resp_json.dump());
            return;
        }
        utils::JWT jwt = utils::JWT("secret");
        int uid = 0;
        int role = 0;
        if(jwt.verifyJWT(token,uid,role)){
            // 列出所有文件
            SubTask *task = tlbFileService_.list_all(uid,[resp,sw,resp_json](bool ok,std::string msg, std::vector<models::TlbFile> files)mutable {
                if(ok){
                    resp_json["status"] = "success";
                    resp_json["message"] = "获取文件列表成功";
                    resp_json["data"]["files"] = files;
                    resp->Json(resp_json.dump());
                }else{
                    resp->set_status_code("500");
                    resp_json["status"] = "error";
                    resp_json["message"] = msg;
                    resp->Json(resp_json.dump());
                }
            });
            sw->push_back(task);
        }else
        {
            resp->set_status_code("401"); // 401 Unauthorized
            resp_json["status"] = "error";
            resp_json["message"] = "无效的 Token";
            resp->Json(resp_json.dump());
            return;
        }
    }
}
