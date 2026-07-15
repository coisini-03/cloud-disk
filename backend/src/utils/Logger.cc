#include <utils/Logger.hpp>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <filesystem>

namespace utils {

// 静态日志实例初始化（定义）
std::shared_ptr<spdlog::logger> Logger::instance_;

// 初始化日志系统：创建目录、设置双输出（控制台+每日文件）、日志级别和格式
void Logger::init(const std::string& path, const std::string& level, int hour, int minute) {
    // 如果日志文件路径包含父目录，先递归创建目录
    std::filesystem::path p(path);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }

    // 创建控制台彩色输出 sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // 创建每日文件切割 sink（按 hour:minute 时间切割）
    auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(path, hour, minute);
    // 创建名为 "netdisk" 的 logger，同时关联控制台和文件两个 sink
    instance_ = std::make_shared<spdlog::logger>("netdisk", spdlog::sinks_init_list{console_sink, file_sink});
    // 设置日志级别（字符串转枚举，如 "info" / "debug" / "warn" / "error"）
    instance_->set_level(spdlog::level::from_str(level));
    // 设置遇到 info 及以上级别时自动 flush 到文件
    instance_->flush_on(spdlog::level::info);
    // 设置日志输出格式：[时间] [级别] 消息内容
    instance_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
}

// 获取日志实例（单例），若未初始化则返回空指针
std::shared_ptr<spdlog::logger> Logger::get() {
    return instance_;
}

} // namespace utils
