#pragma once
#include <string>
namespace models
{
    struct TlbFile{
        int id;
        int uid;
        std::string filename;
        std::string hashcode;
        int size;
        std::string created_at;
        std::string last_update;
    };
    struct TlbFileContext
    {
        int id;
    };
} // namespace models
