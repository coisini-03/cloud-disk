#include <dao/TlbFileDao.hpp>
#include <fmt/core.h>
#include <utils/PreSql.hpp>
#include <workflow/MySQLResult.h>
namespace dao{
    TlbFileDao::TlbFileDao(const std::string& url_path)
        : url_path_(url_path)
    {
    }
    TlbFileDao::~TlbFileDao() = default;

    // 上传文件
    WFMySQLTask* TlbFileDao::insert(const models::TlbFile& file,std::function<void(bool ok, std::string msg)> on_complete)
    {
        std::string sql = fmt::format(
            "INSERT INTO tbl_file (uid, filename, hashcode, size) VALUES ({}, '{}', '{}', {})",
            std::to_string(file.uid),
            utils::PreSql::escape_sql(file.filename),
            utils::PreSql::escape_sql(file.hashcode),
            std::to_string(file.size));

        WFMySQLTask *task = WFTaskFactory::create_mysql_task(url_path_, 3,[on_complete](WFMySQLTask *task){
            // 检查网络是否成功
            if(task->get_state() != WFT_STATE_SUCCESS){
                on_complete(false,task->get_resp()->get_error_msg());
                return;
            }

            // 检查数据库是否成功
            if(task->get_resp()->get_packet_type() == MYSQL_PACKET_ERROR){
                on_complete(false,task->get_resp()->get_error_msg());
                return;
            }
            // 获取上下文
            models::TlbFileContext *file_ctx = static_cast<models::TlbFileContext*>(series_of(task)->get_context());
            protocol::MySQLResultCursor cursor(task->get_resp());
            file_ctx->id = cursor.get_insert_id();
            on_complete(true,"");
        });
        task->get_req()->set_query(sql);
        return task;
    }

    // 查询所有文件
    
    

}

