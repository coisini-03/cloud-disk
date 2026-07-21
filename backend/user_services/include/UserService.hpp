#pragma once

#include <workflow/SubTask.h>
#include <string>
#include <functional>
#include "UserDao.hpp"

namespace services
{
    class UserService
    {
    public:
        UserService(dao::UserDao &userDao);
        ~UserService() = default;

        WFMySQLTask *login_user(const std::string &user_name, const std::string &password,std::function<void(bool ok,std::string msg,std::string token)> on_complete);
        WFMySQLTask *register_user(const std::string &user_name, const std::string &password, std::function<void(bool ok,std::string msg)> on_complete);
        WFMySQLTask *get_user_info(const int &id, std::function<void(bool ok,std::string msg,models::SysUser userInfo)> on_complete);
    private:
        dao::UserDao &userDao_;
    };
}