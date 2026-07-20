#include <dao/TlbFileDao.hpp>
#include <fmt/core.h>
#include "PreSql.hpp"
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
            if(file_ctx){
                file_ctx->id = cursor.get_insert_id();
            }
            on_complete(true,"");
        });
        task->get_req()->set_query(sql);
        return task;
    }

    // 查询所有文件
    WFMySQLTask* TlbFileDao::select_all(int uid,std::function<void(bool ok, std::string msg,std::vector<models::TlbFile> files)> on_complete)
    {
        std::string sql = fmt::format(
            "SELECT id, filename, hashcode, size, created_at, last_update FROM tbl_file WHERE uid = {}",
            std::to_string(uid)
            );
        WFMySQLTask *task = WFTaskFactory::create_mysql_task(url_path_, 3,[on_complete](WFMySQLTask *task){
            std::vector<models::TlbFile> files;
            // 检查网络是否成功
            if(task->get_state() != WFT_STATE_SUCCESS){
                on_complete(false,task->get_resp()->get_error_msg(),files);
                return;
            }

            // 检查数据库是否成功
            protocol::MySQLResultCursor cursor(task->get_resp());
            if(cursor.get_cursor_status() != MYSQL_STATUS_GET_RESULT && 
               cursor.get_cursor_status() != MYSQL_STATUS_OK) {
                on_complete(false, "Cursor status error", files);
                return;
            }
            
            // 解析查询结果
            std::map<std::string,protocol::MySQLCell> row_map;
            while(cursor.fetch_row(row_map)){
                models::TlbFile file;

                file.id = row_map["id"].as_int();
                file.uid = row_map["uid"].as_int();
                file.filename = row_map["filename"].as_string();
                file.hashcode = row_map["hashcode"].as_string();
                file.size = row_map["size"].as_int();
                file.created_at = row_map["created_at"].as_string();
                file.last_update = row_map["last_update"].as_string();

                files.push_back(file);
            }
            on_complete(true,"",files);
        });
        task->get_req()->set_query(sql);
        return task;
    }

    // 根据文件ID查询文件信息
    WFMySQLTask* TlbFileDao::select_by_id(int file_id,std::function<void(bool ok, std::string msg,models::TlbFile file)> on_complete)
    {
        std::string sql = fmt::format(
            "SELECT id, uid, filename, hashcode, size, created_at, last_update FROM tbl_file WHERE id = {}",
            std::to_string(file_id)
            );

        WFMySQLTask *task = WFTaskFactory::create_mysql_task(url_path_,3,[on_complete](WFMySQLTask *task){
            models::TlbFile file;
            // 检查网络是否成功
            if(task->get_state() != WFT_STATE_SUCCESS){
                on_complete(false,task->get_resp()->get_error_msg(),file);
                return;
            }

            // 检查数据库是否成功
            protocol::MySQLResultCursor cursor(task->get_resp());
            if(cursor.get_cursor_status() != MYSQL_STATUS_GET_RESULT && 
               cursor.get_cursor_status() != MYSQL_STATUS_OK) {
                on_complete(false, "Cursor status error", file);
                return;
            }
            
            // 获取查询结果
            std::map<std::string,protocol::MySQLCell> row_map;
            if(cursor.fetch_row(row_map)){
                file.id = row_map["id"].as_int();
                file.uid = row_map["uid"].as_int();
                file.filename = row_map["filename"].as_string();
                file.hashcode = row_map["hashcode"].as_string();
                file.size = row_map["size"].as_int();
                file.created_at = row_map["created_at"].as_string();
                file.last_update = row_map["last_update"].as_string();
            }
            on_complete(true,"",file);
        });
        task->get_req()->set_query(sql);
        return task;
    }
}
