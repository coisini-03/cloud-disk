#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
// 替换为你的实际 Config 头文件路径，例如 "Config.hpp"
#include "utils/Config.hpp" 

using namespace utils; 
using namespace std;
void runManualConfigTests() {
    std::cout << "========== 开始手动测试 Config 模块 ==========\n" << std::endl;

    std::string test_file = "/home/coisini/wangdao/code/cloud-disk/backend/conf/app.json";

    // 2. 加载配置文件
    std::cout << "[测试 1] 初始化 Config 对象" << std::endl;
    Config* config = nullptr;
    try {
        config = new Config(test_file);
        std::cout << "  ✅ 成功: 成功解析 JSON 文件。\n" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  ❌ 失败: 解析 JSON 时发生异常 - " << e.what() << "\n" << std::endl;
        return;
    }

    // 3. 测试获取基础和嵌套的值 (不带默认值)
    std::cout << "[测试 2] 读取存在的配置项 (点分路径)" << std::endl;
    try {
        std::string app_name = config->get<std::string>("app_name");
        int port = config->get<int>("server.port");
        bool ssl = config->get<bool>("server.enable_ssl");

        if (app_name == "CloudDisk_Backend" && port == 8080 && ssl == false) {
            cout << app_name << endl;
            cout << port << endl;
            cout << ssl << endl;
            std::cout << "  ✅ 成功: 配置项读取正确 (app_name=" << app_name << ", port=" << port << ").\n" << std::endl;
        } else {
            std::cout << "  ❌ 失败: 读取的值与预期不符！\n" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "  ❌ 失败: 读取存在的键时抛出了异常 - " << e.what() << "\n" << std::endl;
    }

    // 4. 测试获取不存在的值并使用默认值
    std::cout << "[测试 3] 读取不存在的配置项 (使用默认值)" << std::endl;
    try {
        // 尝试读取一个不存在的 worker_threads，期望返回我们给的默认值 4
        int threads = config->get<int>("server.worker_threads", 4);
        std::string unknown = config->get<std::string>("not.exist.key", "default_string");

        if (threads == 4 && unknown == "default_string") {
            std::cout << "  ✅ 成功: 默认值机制生效。\n" << std::endl;
        } else {
            std::cout << "  ❌ 失败: 没有返回正确的默认值！\n" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "  ❌ 失败: 使用默认值接口时不应该抛出异常 - " << e.what() << "\n" << std::endl;
    }

    // 5. 测试获取不存在的值 (无默认值，预期抛出异常)
    std::cout << "[测试 4] 读取不存在的配置项 (预期抛出异常)" << std::endl;
    try {
        config->get<int>("database.redis.port"); // 这个键根本不存在
        std::cout << "  ❌ 失败: 试图读取不存在的键，但没有抛出异常！\n" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  ✅ 成功: 正确地捕获了缺失键的异常 - " << e.what() << "\n" << std::endl;
    }

    // 6. 测试 MySQL URL 拼接
    std::cout << "[测试 5] 生成 MySQL URL" << std::endl;
    try {
        std::string url = config->mysql_url();
        std::cout << "  ✅ 成功: 生成的 URL 为 -> " << url << "\n" << std::endl;
        // 如果你的 URL 格式是 mysql://user:pass@host:port/db
        // 预期输出应该是: mysql://root:123456@127.0.0.1:3306/netdisk_db
    } catch (const std::exception& e) {
        std::cout << "  ❌ 失败: 生成 MySQL URL 时抛出异常 - " << e.what() << "\n" << std::endl;
    }

    // 清理资源
    delete config;
    std::cout << "========== Config 模块测试结束 ==========" << std::endl;
}

int main() {
    runManualConfigTests();
    return 0;
}