#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
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

#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(utils::Logger::get(), __VA_ARGS__) // 调试日志
#define LOG_INFO(...)  SPDLOG_LOGGER_INFO(utils::Logger::get(), __VA_ARGS__)  // 普通信息日志
#define LOG_WARN(...)  SPDLOG_LOGGER_WARN(utils::Logger::get(), __VA_ARGS__)  // 警告日志
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(utils::Logger::get(), __VA_ARGS__) // 错误日志
