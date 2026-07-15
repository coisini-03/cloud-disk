#include <dao/UserDao.hpp>
#include <utils/PreSql.hpp>
#include <workflow/WFTaskFactory.h>
#include <workflow/MySQLResult.h>
#include <fmt/core.h>
namespace dao
{
    UserDao::UserDao(const std::string &url_path)
        : url_path_(url_path)
    {
    }
    UserDao::~UserDao() = default;

    // 插入
    WFMySQLTask* UserDao::insert(const models::SysUser &user,std::function<void(bool ok, std::string msg)> on_complete)
    {
        std::string user_name = utils::PreSql::escape_sql(user.user_name);
        std::string password_hash = utils::PreSql::escape_sql(user.password_hash);

        std::string sql = fmt::format(
            "INSERT INTO sys_user (user_name, password_hash) VALUES ('{}','{}')",
            user_name,password_hash
            );

        WFMySQLTask *task = WFTaskFactory::create_mysql_task(url_path_, 3,[on_complete](WFMySQLTask *task){
            if(task->get_state() != WFT_STATE_SUCCESS){
                on_complete(false,"network error");
                return;
            }
            protocol::MySQLResultCursor cursor(task->get_resp());
            if(cursor.get_cursor_status() == MYSQL_STATUS_ERROR){
                on_complete(false,"db error");
                return ;
            }
            on_complete(true,"");
            
        });
        task->get_req()->set_query(sql);
        return task;
    }
}