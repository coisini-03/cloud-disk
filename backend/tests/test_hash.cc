#include <iostream>
#include <string>
#include <stdexcept>
// 替换为你的实际 Hash 头文件路径，例如 "Hash.hpp"
#include "utils/Hash.hpp" 

using namespace std;
void runManualHashTests() {
    std::cout << "========== 开始手动测试 Hash 模块 ==========\n" << std::endl;

    // ---------------------------------------------------------
    // 测试用例 1：测试基础数据哈希 (generate_hashcode)
    // ---------------------------------------------------------
    std::cout << "[测试 1] 基础数据哈希 (generate_hashcode)" << std::endl;
    std::string data1 = "hello world";
    std::string data2 = "hello world";
    std::string data3 = "hello C++";

    std::string hash1 = utils::Hash::generate_hashcode(reinterpret_cast<const unsigned char*>(data1.data()), data1.size());
    std::string hash2 = utils::Hash::generate_hashcode(reinterpret_cast<const unsigned char*>(data2.data()), data2.size());
    std::string hash3 = utils::Hash::generate_hashcode(reinterpret_cast<const unsigned char*>(data3.data()), data3.size());

    if (!hash1.empty() && hash1 == hash2 && hash1 != hash3) {
        cout << "hash1: " << hash1 << endl;
        cout << "hash2: " << hash2 << endl;
        cout << "hash3: " << hash3 << endl;
        std::cout << "  ✅ 成功: 相同数据生成的哈希一致，不同数据哈希不同。\n" << std::endl;
    } else {
        std::cout << "  ❌ 失败: 基础哈希逻辑（一致性/差异性）有误！\n" << std::endl;
    }

    // ---------------------------------------------------------
    // 测试用例 2：测试密码哈希与验证逻辑
    // ---------------------------------------------------------
    std::cout << "[测试 2] 密码哈希与验证 (hash_password / verify_password)" << std::endl;
    std::string password = "MySuperSecretPassword123!";
    std::string wrong_password = "WrongPassword456!";

    std::string pwhash = utils::Hash::hash_password(password);

    if (pwhash.empty()) {
        std::cout << "  ❌ 失败: 生成的密码哈希为空！\n" << std::endl;
    } else {
        bool is_valid = utils::Hash::verify_password(password, pwhash);
        bool is_invalid = utils::Hash::verify_password(wrong_password, pwhash);

        if (is_valid && !is_invalid) {
            std::cout << "  ✅ 成功: 正确密码验证通过，错误密码被成功拦截。\n" << std::endl;
        } else {
            std::cout << "  ❌ 失败: 密码验证逻辑有误！\n" << std::endl;
        }
    }

    // ---------------------------------------------------------
    // 测试用例 3：测试密码哈希的随机“加盐”特性
    // ---------------------------------------------------------
    std::cout << "[测试 3] 密码哈希加盐特性" << std::endl;
    std::string hash_salt1 = utils::Hash::hash_password(password);
    std::string hash_salt2 = utils::Hash::hash_password(password);

    if (hash_salt1 != hash_salt2) {
        bool verify1 = utils::Hash::verify_password(password, hash_salt1);
        bool verify2 = utils::Hash::verify_password(password, hash_salt2);
        
        if (verify1 && verify2) {
            std::cout << "  ✅ 成功: 同一密码两次生成的密文不同（加盐生效），但均能验证通过。\n" << std::endl;
        } else {
            std::cout << "  ❌ 失败: 加盐密文验证失败！\n" << std::endl;
        }
    } else {
        std::cout << "  ❌ 失败: 两次哈希密文完全相同，未生效随机加盐机制！\n" << std::endl;
    }

    std::cout << "========== Hash 模块测试结束 ==========" << std::endl;
}

int main() {
    try {
        runManualHashTests();
    } catch (const std::exception& e) {
        // 捕获可能抛出的异常，例如 libsodium 初始化失败
        std::cerr << "发生严重异常: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}