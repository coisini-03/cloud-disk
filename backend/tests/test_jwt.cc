#include <iostream>
#include <string>
// 这里替换成你实际的头文件路径，比如 "JWT.hpp"
#include "utils/JWT.hpp" 

using namespace utils;

void runManualTests() {
    std::cout << "========== 开始手动测试 JWT 模块 ==========\n" << std::endl;

    // --- 准备全局测试数据 ---
    std::string secret = "test_secret_key_123";
    JWT jwt(secret);
    std::string generated_token;

    // --- 测试用例 1：生成 JWT ---
    std::cout << "[测试 1] 正常生成 JWT" << std::endl;
    int input_user_id = 1024;
    int input_role = 1;
    int expire_s = 3600;
    
    // 注意：接口是引用传参，必须传变量
    generated_token = jwt.generateJWT(input_user_id, input_role, expire_s);
    
    if (!generated_token.empty()) {
        std::cout << "  ✅ 成功: 生成了 Token (" << generated_token << ")\n" << std::endl;
    } else {
        std::cout << "  ❌ 失败: 生成的 Token 为空！\n" << std::endl;
        return; // 生成失败的话，后续验证也做不了，直接退出
    }

    // --- 测试用例 2：验证合法的 JWT ---
    std::cout << "[测试 2] 验证刚才生成的合法 Token" << std::endl;
    std::string output_user_id;
    int output_role = 0;
    
    bool is_valid = jwt.verifyJWT(generated_token, output_user_id, output_role);
    
    // 验证逻辑：不仅要 verifyJWT 返回 true，解析出来的数据也要和输入时一致
    if (is_valid && output_user_id == std::to_string(input_user_id) && output_role == input_role) {
        std::cout << "  ✅ 成功: 验证通过！" << std::endl;
        std::cout << "     - 解析的 User ID: " << output_user_id << std::endl;
        std::cout << "     - 解析的 Role: " << output_role << "\n" << std::endl;
    } else {
        std::cout << "  ❌ 失败: 验证未通过，或解析数据不一致！\n" << std::endl;
    }

    // --- 测试用例 3：使用错误的密钥验证 ---
    std::cout << "[测试 3] 使用错误的密钥进行验证" << std::endl;
    JWT bad_jwt("wrong_secret_key");
    std::string temp_user_id;
    int temp_role = 0;
    
    bool bad_valid = bad_jwt.verifyJWT(generated_token, temp_user_id, temp_role);
    
    if (!bad_valid) {
        std::cout << "  ✅ 成功: 错误密钥被正确拦截，返回 false。\n" << std::endl;
    } else {
        std::cout << "  ❌ 失败: 错误密钥居然验证通过了！\n" << std::endl;
    }

    // --- 测试用例 4：验证被篡改的 Token ---
    std::cout << "[测试 4] 验证被篡改的 Token" << std::endl;
    std::string tampered_token = generated_token;
    tampered_token.back() = tampered_token.back() == 'a' ? 'b' : 'a'; // 简单篡改最后一个字符
    
    std::string temp_user_id_2;
    int temp_role_2 = 0;
    
    bool tampered_valid = jwt.verifyJWT(tampered_token, temp_user_id_2, temp_role_2);
    
    if (!tampered_valid) {
        std::cout << "  ✅ 成功: 被篡改的 Token 被正确拦截。\n" << std::endl;
    } else {
        std::cout << "  ❌ 失败: 被篡改的 Token 居然验证通过了！\n" << std::endl;
    }

    std::cout << "========== JWT 模块测试结束 ==========" << std::endl;
}

int main() {
    runManualTests();
    return 0;
}