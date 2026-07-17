#include <services/TlbFileService.hpp>
#include <dao/TlbFileDao.hpp>
#include <models/TlbFile.hpp>
#include <utils/Hash.hpp>
#include <workflow/WFTaskFactory.h>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <memory>

namespace services
{
    TlbFileService::TlbFileService(dao::TlbFileDao &tlbFileDao)
        : tlbFileDao_(tlbFileDao)
    {
    }
    TlbFileService::~TlbFileService() = default;

    SubTask *TlbFileService::upload(int uid, std::string physical_path, std::string content, off_t offset, std::function<void(bool ok, std::string msg)> on_complete)
    {
        // 打开文件
        int fd = open(physical_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            on_complete(false, "无法打开/创建文件");
            return WFTaskFactory::create_empty_task();
        } 
        models::TlbFile file;
        auto shared_content = std::make_shared<std::string>(content);
        size_t file_size = shared_content->size();
        file.uid = uid;
        file.hashcode = utils::Hash::generate_hashcode(reinterpret_cast<const unsigned char *>(shared_content->c_str()),file_size);
        file.filename = std::filesystem::path(physical_path).filename().string();
      
        // 写入文件
        WFFileIOTask *task = WFTaskFactory::create_pwrite_task(fd, shared_content->c_str(), file_size, offset, [shared_content,file,this,fd,on_complete](WFFileIOTask *task) 
        {
            // 关闭文件
            close(fd);
            // 调用回调函数
            if(task->get_state() != WFT_STATE_SUCCESS){
                on_complete(false,"network error");
                return;
            }
            WFMySQLTask *mysql_task = tlbFileDao_.insert(file,[on_complete](bool ok,std::string msg){
                on_complete(ok,msg);
            });
            series_of(task)->push_back(mysql_task);
        });
        return task;
    }
    SubTask *TlbFileService::list_all(int uid,std::function<void(bool ok,std::string msg,std::vector<models::TlbFile> files)> on_complete){
        SubTask *task = tlbFileDao_.select_all(uid,[on_complete](bool ok,std::string msg,std::vector<models::TlbFile> files){
            on_complete(true,msg,files);
        });
        return task;
    }
}
