#include <iostream>
#include <string>
#include <exception>
#include <fstream>
// 替换为你实际的 Logger 头文件路径，例如 "Logger.hpp"
#include "utils/Logger.hpp" 

void runManualLoggerTests() {
    std::cout << "========== 开始手动测试 Logger 模块 ==========\n" << std::endl;

    // ---------------------------------------------------------
    // 测试用例 1：初始化 Logger
    // ---------------------------------------------------------
    std::cout << "[测试 1] 初始化 Logger (Logger::init)" << std::endl;
    
    // 设置日志文件名为 test_log.txt，日志级别为 debug
    // 注意：spdlog 的 daily_logger 通常会自动在文件名后追加日期，例如 test_log_2026-07-15.txt
    std::string log_path = "test_log.txt"; 
    std::string log_level = "info";

    try {
        // 每天 0 点 0 分切割日志
        utils::Logger::init(log_path, log_level, 0, 0);
        
        // 验证单例是否成功创建
        if (utils::Logger::get() != nullptr) {
            std::cout << "  ✅ 成功: Logger 初始化完成，实例非空。\n" << std::endl;
        } else {
            std::cout << "  ❌ 失败: Logger 初始化后，实例仍为空！\n" << std::endl;
            return; // 实例为空后续无法测试，直接退出
        }
    } catch (const std::exception& e) {
        std::cout << "  ❌ 失败: 初始化抛出异常 - " << e.what() << "\n" << std::endl;
        return;
    }

    // ---------------------------------------------------------
    // 测试用例 2：测试日志宏输出与格式化
    // ---------------------------------------------------------
    std::cout << "[测试 2] 测试宏输出 (LOG_DEBUG, LOG_INFO, ...)" << std::endl;
    try {
        // spdlog 底层使用 fmt 库，支持 {} 占位符格式化
        LOG_DEBUG("这是一条 [Debug] 级别的测试日志，测试数字: {}", 1024);
        LOG_INFO("这是一条 [Info] 级别的测试日志，测试字符串: {}", "Hello Netdisk");
        LOG_WARN("这是一条 [Warn] 级别的测试日志，测试浮点数: {:.2f}", 3.14159);
        LOG_ERROR("这是一条 [Error] 级别的测试日志，发生了一个严重的错误！");
        
        // 强制刷新日志到磁盘（如果你的 init 里没有设置自动 flush 的话）
        utils::Logger::get()->flush();

        std::cout << "  ✅ 成功: 日志宏调用未崩溃。" << std::endl;
        std::cout << "     -> 请检查当前目录下是否生成了类似 test_log_YYYY-MM-DD.txt 的文件。" << std::endl;
        std::cout << "     -> 并且检查文件内容是否包含上述 4 条日志信息。\n" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  ❌ 失败: 写入日志时抛出异常 - " << e.what() << "\n" << std::endl;
    }

    std::cout << "========== Logger 模块测试结束 ==========" << std::endl;
}

int main() {
    runManualLoggerTests();
    return 0;
}