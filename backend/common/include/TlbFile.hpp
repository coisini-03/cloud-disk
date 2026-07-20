#pragma once
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

namespace models
{
    using json = nlohmann::json;
    struct TlbFile{
        int id;
        int uid;
        std::string filename;
        std::string hashcode;
        int size;
        std::string created_at;
        std::string last_update;

        friend
        inline std::ostream &operator<<(std::ostream &os, const TlbFile &file)
        {
            os << "[TlbFile]: " << file.filename
               << ", hashcode: " << file.hashcode // 修复1：使用 file.hashcode
               << ", size: " << file.size
               << ", created_at: " << file.created_at    // 修复2：create_at 改为 created_at
               << ", last_update: " << file.last_update; // 修复3：去掉了结尾多余的 <<

            return os;
        }
    };
    inline void to_json(json& j, const TlbFile& f) {
    j = json{
            {"fileId", f.id},
            {"filename", f.filename},
            {"size", f.size},
            {"createdAt", f.created_at},
            {"updatedAt", f.last_update}
        };
    }
    struct TlbFileContext
    {
        int id;
        std::string filename;
    };
} // namespace models
