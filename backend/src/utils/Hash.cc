#include "utils/Hash.hpp"
#include <jwt.h>
#include <sodium/crypto_hash_sha256.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>

using namespace std;

namespace utils
{
    // 链接属性
    // 内部链接: static
    // 外部链接：默认
    static const char *SECRET_KEY = "$^Hk16NV";

    Hash::SodiumInitializer initializer;

    std::string Hash::generate_hashcode(const unsigned char *data, size_t n)
    {
        unsigned char output[crypto_hash_sha256_BYTES];
        if (crypto_hash_sha256(output, data, n) != 0)
        {
            throw std::runtime_error("生成SHA256哈希失败");
        }
        // 转换成十六进制字符，存储到result中
        char result[crypto_hash_sha256_BYTES * 2 + 1] = "";
        for (unsigned i = 0; i < crypto_hash_sha256_BYTES; ++i)
        {
            sprintf(result + 2 * i, "%02x", output[i]);
        }
        return result;
    }

    string Hash::hash_password(const string &password)
    {
        // 输出缓冲区，用于存储生成的哈希字符串，必须为`crypto_pwhash_STRBYTES`字节 (128字节)
        char hashed[crypto_pwhash_STRBYTES];

        if (crypto_pwhash_str(
                hashed,
                password.c_str(),
                password.length(),
                crypto_pwhash_OPSLIMIT_INTERACTIVE, // 操作限制(operation limit)
                crypto_pwhash_MEMLIMIT_INTERACTIVE) // 内存限制(memory limit)
            != 0)
        {
            throw std::runtime_error("生成密码的哈希值失败");
        }
        return string(hashed);
    }

    bool Hash::verify_password(const string &password, const string &pwhash)
    {
        return crypto_pwhash_str_verify(pwhash.c_str(),
                                        password.c_str(),
                                        password.length()) == 0;
    }
}
