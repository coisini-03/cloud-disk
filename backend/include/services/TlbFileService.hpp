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

    private:
        dao::TlbFileDao &tlbFileDao_;
    };
}
