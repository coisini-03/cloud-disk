
#include <services/UserService.hpp>
#include <utils/Hash.hpp>
namespace services
{
    UserService::UserService(dao::UserDao &userDao)
        : userDao_(userDao)
    {
    }

    SubTask *UserService::login_user(const std::string &user_name, const std::string &password,std::function<void(bool ok,std::string msg)> on_complete)
    {
        SubTask *task = userDao_.select_by_username(user_name,[on_complete,password](bool ok,std::string msg,models::SysUser &user)
        {
            if(ok){
                if(utils::Hash::verify_password(password,user.password_hash)){
                    on_complete(true,"");
                }
                else{
                    on_complete(false,"密码错误");
                }
            }
            else{
                on_complete(false,msg);
            }
        });
        return task;
    }

    WFMySQLTask *UserService::register_user(const std::string &user_name, const std::string &password,std::function<void(bool ok,std::string msg)> on_complete)
    {
        models::SysUser new_user = {user_name,utils::Hash::hash_password(password)};
        WFMySQLTask *task = userDao_.insert(new_user, [on_complete](bool ok, std::string msg)
        { 
            on_complete(ok, msg); 
        });
        return task;
    }
}