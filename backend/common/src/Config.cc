#include "Config.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace utils
{

    Config::Config(const std::string &path)
    {
        std::ifstream ifs(path);
        if (!ifs)
        {
            throw std::runtime_error("Cannot open config file: " + path);
        }
        json_ = nlohmann::json::parse(ifs);
    }

    // 内部辅助函数：将 "database.host" 转换为 JSON Pointer "/database/host"
    nlohmann::json::json_pointer Config::to_pointer(const std::string &key)
    {
        std::string ptr_str = "/" + key;
        std::replace(ptr_str.begin(), ptr_str.end(), '.', '/');
        return nlohmann::json::json_pointer(ptr_str);
    }

    std::string Config::mysql_url() const
    {
        // 这里的业务逻辑无需任何更改
        std::string host = get<std::string>("database.host", "127.0.0.1");
        int port = get<int>("database.port", 3306);
        std::string user = get<std::string>("database.user", "root");
        std::string pass = get<std::string>("database.password", "");
        std::string db = get<std::string>("database.db", "netdisk");
        return "mysql://" + user + ":" + pass + "@" + host + ":" + std::to_string(port) + "/" + db;
    }

} // namespace utils
