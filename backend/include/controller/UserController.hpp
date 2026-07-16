#pragma once
#include <wfrest/BluePrint.h>
#include <wfrest/HttpMsg.h>
#include <services/UserService.hpp>

namespace controller { 

    class UserController {
    public:
        explicit UserController(services::UserService &userService);
        ~UserController();
        // 注册路由
        void registerRoutes(wfrest::BluePrint &bp);
  

    private:
        services::UserService &userService_;
        // 注册用户
        void registerHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw);
        // 登录用户
        void loginHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw);
        // 获取当前用户信息
        void getUserHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw);
    };

    
}
