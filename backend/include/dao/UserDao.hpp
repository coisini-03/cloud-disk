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
        // WFTaskFactory* select_by_id(const int user_id, models::SysUser& user);
        // WFTaskFactory* select_by_username(const std::string& username, models::SysUser& user);
        // WFTaskFactory* update_password(const int user_id, const std::string& password);
        // WFTaskFactory* delete_by_id(const int user_id);
    private:
        std::string url_path_;
    };
}