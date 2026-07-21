#pragma once
#include "UserService.hpp"
#include "user.srpc.h"


namespace controller { 

    class UserController : public cloud_disk::UserService::Service {
    public:
        explicit UserController(services::UserService &userService);
        ~UserController();

        // 登录
        void Login(cloud_disk::LoginRequest *req, cloud_disk::LoginResponse *resp,srpc::RPCContext *ctx) override;
        // 注册用户
        void Register(cloud_disk::RegisterRequest *req, cloud_disk::RegisterResponse *resp,srpc::RPCContext *ctx) override;
        // 获取当前用户信息
        void GetUser(cloud_disk::GetUserRequest *req, cloud_disk::GetUserResponse *resp,srpc::RPCContext *ctx) override;
    private:
        services::UserService &userService_;
        // 注册用户
        // void registerHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw);
        // // 获取当前用户信息
        // void getUserHandler(const wfrest::HttpReq *req, wfrest::HttpResp *resp,SeriesWork *sw);
    };

    
}
