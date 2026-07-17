#pragma once
#include <dao/TlbFileDao.hpp>
#include <models/TlbFile.hpp>
namespace services
{
    class TlbFileService
    {
    public:
        TlbFileService(dao::TlbFileDao &tlbFileDao);
        ~TlbFileService();
        SubTask *upload(int uid, std::string physical_path, std::string content, off_t offset, std::function<void(bool ok, std::string msg)> done);
        SubTask *list_all(int uid,std::function<void(bool ok,std::string msg,std::vector<models::TlbFile> files)> on_complete);
        SubTask *download(int file_id,std::function<void(bool ok,std::string msg,std::string content)> on_complete);
    private:
        dao::TlbFileDao &tlbFileDao_;
    };
}
