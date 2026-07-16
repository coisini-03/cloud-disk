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

    // 插入用户
    WFMySQLTask* UserDao::insert(const models::SysUser &user,std::function<void(bool ok, std::string msg)> on_complete)
    {
        std::string user_name = utils::PreSql::escape_sql(user.user_name);
        std::string password_hash = utils::PreSql::escape_sql(user.password_hash);

        std::string sql = fmt::format(
            "INSERT INTO sys_user (user_name, password_hash) VALUES ('{}','{}')",
            user_name,password_hash
            );

        WFMySQLTask *task = WFTaskFactory::create_mysql_task(url_path_, 3,[on_complete](WFMySQLTask *task)
        {
            // 检查网络错误
            if(task->get_state() != WFT_STATE_SUCCESS){
                on_complete(false,"network error");
                return;
            }

            if(task->get_resp()->get_packet_type() == MYSQL_PACKET_ERROR){
                int error_code = task->get_resp()->get_error_code();
                std::string error_msg = task->get_resp()->get_error_msg();
                if(error_code == 1062){
                    on_complete(false,"用户名已存在");
                    return ;
                }
                on_complete(false,fmt::format("db error: {} {}",error_code,error_msg));
                return ;
            }
            
            // 获取上下文
            models::UserContext *user = static_cast<models::UserContext*>(series_of(task)->get_context());
            protocol::MySQLResultCursor cursor(task->get_resp());
            user->user_id = cursor.get_insert_id();
            on_complete(true,"");
            
        });
        task->get_req()->set_query(sql);
        return task;
    }
    // 根据用户名查询用户信息
    WFMySQLTask* UserDao::select_by_username(const std::string& username,std::function<void(bool ok, std::string msg, models::SysUser &user)> on_complete)
    {
        std::string user_name = utils::PreSql::escape_sql(username);
        std::string sql = fmt::format(
            "SELECT * FROM sys_user WHERE user_name = '{}'",
            user_name
            );

        WFMySQLTask *task = WFTaskFactory::create_mysql_task(url_path_, 3,[on_complete](WFMySQLTask *task)
        {
            models::SysUser user;
            // 检查网络错误
            if(task->get_state() != WFT_STATE_SUCCESS){
                on_complete(false,"network error",user);
                return;
            }

            std::vector<protocol::MySQLCell> cells;
            protocol::MySQLResultCursor cursor(task->get_resp());
            if(cursor.get_cursor_status() == MYSQL_STATUS_OK ){   
                std::string err_msg = task->get_resp()->get_error_msg();
                on_complete(false,"db execute error:" + err_msg,user);
                return ;
            }

            std::unordered_map<std::string, protocol::MySQLCell> row_map;
            if(cursor.fetch_row(row_map)){
                user.id = row_map["id"].as_int();
                user.user_name = row_map["user_name"].as_string();
                user.password_hash = row_map["password_hash"].as_string();
                user.role = row_map["role"].as_int();
                user.create_time = row_map["create_time"].as_string();
                user.use_capacity = row_map["use_capacity"].as_ulonglong();
                user.total_capacity = row_map["total_capacity"].as_ulonglong();
                user.status = row_map["status"].as_int();
                models::UserContext *user_ctx = static_cast<models::UserContext*>(series_of(task)->get_context());
                user_ctx->user_id = user.id;
                user_ctx->user_name = user.user_name;
                on_complete(true,"",user);
            }else{
                on_complete(false,"用户不存在",user);   
            }
        });
        task->get_req()->set_query(sql);
        return task;
    }
}