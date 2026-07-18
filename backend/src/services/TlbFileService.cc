#include <services/TlbFileService.hpp>
#include <dao/TlbFileDao.hpp>
#include <models/TlbFile.hpp>
#include <utils/Hash.hpp>
#include <workflow/WFTaskFactory.h>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <memory>
#include <utils/AlibabaOss.hpp>
#include <utils/Logger.hpp>
#include <nlohmann/json.hpp>
#include <utils/RabbitMQ.hpp>


namespace services
{
    const size_t MAX_MEMORY_SIZE = 50 * 1024 * 1024;
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
        std::cout << file_size << std::endl;
        file.uid = uid;
        file.size = file_size;
        file.hashcode = utils::Hash::generate_hashcode(reinterpret_cast<const unsigned char *>(shared_content->c_str()),file_size);
        file.filename = std::filesystem::path(physical_path).filename().string();

        // 写入文件
        WFFileIOTask *task = WFTaskFactory::create_pwrite_task(fd, shared_content->c_str(), file_size, offset, [physical_path,shared_content,file,this,fd,on_complete](WFFileIOTask *task) 
        {
            // 关闭文件
            close(fd);
            // 调用回调函数
            if(task->get_state() != WFT_STATE_SUCCESS){
                LOG_WARN("write file to disk fail network error");
                on_complete(false,"network error");
                return;
            }
            LOG_INFO("write file to disk success");
            // 容灾备份
            nlohmann::json json;
            json["uid"] = file.uid;
            json["filename"] = file.filename;
            json["physical_path"] = physical_path;
            utils::RabbitMQ::getInstance().publish(json.dump());

            WFMySQLTask *mysql_task = tlbFileDao_.insert(file,[on_complete](bool ok,std::string msg){
                if(!ok){
                    LOG_ERROR("insert file to db fail");
                }else{
                    LOG_INFO("insert file to db success");
                }
                on_complete(ok,msg);
            });
            series_of(task)->push_back(mysql_task);
        });
        return task;
    }
    SubTask *TlbFileService::list_all(int uid,std::function<void(bool ok,std::string msg,std::vector<models::TlbFile> files)> on_complete){
        SubTask *task = tlbFileDao_.select_all(uid,[on_complete](bool ok,std::string msg,std::vector<models::TlbFile> files){
            on_complete(ok,msg,files);
        });
        return task;
    }
    SubTask *TlbFileService::download(int file_id,std::function<void(bool ok,std::string msg,std::shared_ptr<std::string> content)> on_complete){
        auto wrap_task = std::make_shared<SubTask *>(nullptr);
        SubTask *task = tlbFileDao_.select_by_id(file_id,[wrap_task,on_complete](bool ok,std::string msg,models::TlbFile file){
            if(!ok){
                on_complete(false,msg,nullptr);
                return;
            }

            std::string base_url = "/home/coisini/wangdao/code/cloud-disk/backend/upload/";
            std::string physical_path = base_url + file.filename;

            // 检查文件是否存在
            if(!std::filesystem::exists(physical_path)){
                on_complete(false,"文件不存在",nullptr);
                return;
            }
            
            std::cout << physical_path << std::endl;
            // 打开文件
            int fd = open(physical_path.c_str(), O_RDONLY);
            std::cout << fd << std::endl;
            if (fd < 0)
            {
                on_complete(false, "无法打开文件",nullptr);
                return;
            } 
            auto buf = std::make_shared<std::string>(MAX_MEMORY_SIZE,'\0');
            // 读取文件
            WFFileIOTask *task = WFTaskFactory::create_pread_task(fd,buf->data(),file.size,0,[fd,buf,on_complete](WFFileIOTask *task){
                close(fd);
                if(task->get_state() != WFT_STATE_SUCCESS){
                    on_complete(false,"network error",nullptr);
                    return;
                }

                if(task->get_retval() < 0){
                    on_complete(false,"read error",nullptr);
                    return;
                }else
                {
                    buf->resize(task->get_retval());
                    on_complete(true,"下载成功",buf);
                }
            });
            series_of(*wrap_task)->push_back(task);
        });
        *wrap_task = task;
        return task;
    }
}
