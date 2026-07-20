
#include <services/UserService.hpp>
#include "Hash.hpp"
#include "JWT.hpp"
#include "SysUser.hpp"
#include <string>
namespace services
{
    UserService::UserService(dao::UserDao &userDao)
        : userDao_(userDao)
    {
    }

    WFMySQLTask *UserService::login_user(const std::string &user_name, const std::string &password,std::function<void(bool ok,std::string msg,std::string token)> on_complete)
    {
        std::string accessToken;
        
        WFMySQLTask *task = userDao_.select_by_username(user_name,[accessToken,on_complete,password](bool ok,std::string msg,models::SysUser user) mutable
        {
            if(ok){
                if(utils::Hash::verify_password(password,user.password_hash)){
                    utils::JWT jwt = utils::JWT("secret");
                    accessToken = jwt.generateJWT(user.id,user.role,60*30);
                    on_complete(true,"登录成功",accessToken);
                }
                else{
                    on_complete(false,"密码错误","");
                }
            }
            else{
                on_complete(false,msg,"");
            }
        });

        return task;
    }

    WFMySQLTask *UserService::register_user(const std::string &user_name, const std::string &password, std::function<void(bool ok,std::string msg)> on_complete)
    {
        // 注册用户
        models::SysUser new_user = {user_name,utils::Hash::hash_password(password)};
        WFMySQLTask *task = userDao_.insert(new_user, [on_complete](bool ok, std::string msg)
        { 
            on_complete(ok, msg); 
        });
        return task;
    }

    WFMySQLTask *UserService::get_user_info(const int &id, std::function<void(bool ok,std::string msg,models::SysUser userInfo)> on_complete)
    {
        WFMySQLTask *task = userDao_.select_by_id(id,[on_complete](bool ok,std::string msg,models::SysUser user) mutable
        {
            on_complete(ok, msg, user);
        });
        return task;
    }
}