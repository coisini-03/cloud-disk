#pragma once
#include <models/SysUser.hpp>
#include <workflow/WFTaskFactory.h>
#include <functional>


namespace dao {
    class UserDao {
    public:
        explicit UserDao(const std::string& url_path);
        ~UserDao();
        WFMySQLTask* insert(const models::SysUser& user,std::function<void(bool ok, std::string msg)> on_complete);
        WFMySQLTask* select_by_id(const int user_id,std::function<void(bool ok, std::string msg,models::SysUser user)> on_complete);
        WFMySQLTask* select_by_username(const std::string& username,std::function<void(bool ok, std::string msg,models::SysUser user)> on_complete);
        // WFTaskFactory* update_password(const int user_id, const std::string& password);
        // WFTaskFactory* delete_by_id(const int user_id);
    private:
        std::string url_path_;
    };
}