#pragma once
#include <spdlog/spdlog.h>
#include <memory>
#include <string>

namespace utils {

// 日志管理类，封装 spdlog 实现全局日志记录
class Logger {
public:
    // 初始化日志系统：设置日志文件路径、级别和每日切割时间
    static void init(const std::string& path, const std::string& level, int hour = 0, int minute = 0);
    // 获取日志实例（单例模式）
    static std::shared_ptr<spdlog::logger> get();

private:
    static std::shared_ptr<spdlog::logger> instance_; // 静态日志实例（单例）
};

} // namespace utils

// 便捷宏定义：不同级别的日志输出
#define LOG_DEBUG(...) utils::Logger::get()->debug(__VA_ARGS__) // 调试日志
#define LOG_INFO(...)  utils::Logger::get()->info(__VA_ARGS__)  // 普通信息日志
#define LOG_WARN(...)  utils::Logger::get()->warn(__VA_ARGS__)  // 警告日志
#define LOG_ERROR(...) utils::Logger::get()->error(__VA_ARGS__) // 错误日志
