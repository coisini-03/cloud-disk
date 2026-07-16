#pragma once
#include <models/TlbFile.hpp>
#include <workflow/WFTaskFactory.h>
#include <functional>
#include <vector>

namespace dao{
    class TlbFileDao{
    public:
        explicit TlbFileDao(const std::string& url_path);
        ~TlbFileDao();
        WFMySQLTask* insert(const models::TlbFile& file,std::function<void(bool ok, std::string msg)> on_complete);
        WFMySQLTask* select_all(std::function<void(bool ok, std::string msg,std::vector<models::TlbFile> files)> on_complete);
    private:
        std::string url_path_;
    };
}