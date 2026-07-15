#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace utils {

// 配置管理类，基于 nlohmann::json 读取 JSON 配置文件
class Config {
public:
    // 构造函数：从指定路径加载 JSON 配置文件
    explicit Config(const std::string& path);

    // 模板方法：获取指定键的配置值（键不存在时抛出异常）
    // 支持点分路径（如 "server.port"）
    template <typename T>
    T get(const std::string &key) const
    {
        try
        {
            // 使用 json_pointer 直接查找，如果不存在或类型错误会抛出异常
            return json_.at(to_pointer(key)).get<T>();
        }
        catch (const nlohmann::json::exception &e)
        {
            throw std::runtime_error("Config key not found or type mismatch: " + key + " (" + e.what() + ")");
        }
    }

    // 模板方法：获取指定键的配置值，若键不存在则返回默认值
    template <typename T>
    T get(const std::string &key, const T &default_value) const
    {
        try
        {
            // json_pointer 的 default value 取法
            return json_.value(to_pointer(key), default_value);
        }
        catch (...)
        {
            return default_value;
        }
    }

    // 组装 MySQL 连接 URL，返回 "mysql://user:pass@host:port/db"
    std::string mysql_url() const;

private:
    nlohmann::json json_; // 存储配置数据的 JSON 对象

    // 辅助函数：按点分路径逐级查找，返回指向目标值的指针（找不到返回 nullptr）
    static nlohmann::json::json_pointer to_pointer(const std::string &key);
};

}// namespace utils
