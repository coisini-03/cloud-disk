
#include "utils/JWT.hpp"
#include <chrono>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

namespace utils
{
    using json_traits = jwt::traits::nlohmann_json;
    JWT::JWT(std::string secret_key)
        : secret_key_(secret_key)
    {
    }
    JWT::~JWT() = default;

    std::string JWT::generateJWT(int &user_id, int role, int &expiretime_s)
    {
        auto now = std::chrono::system_clock::now();
        std::string token = jwt::create<json_traits>()
                                .set_issuer("cloud-disk")
                                .set_type("jws")
                                .set_issued_at(now)
                                .set_expires_at(now + std::chrono::seconds(expiretime_s))
                                .set_payload_claim("user_id", jwt::basic_claim<json_traits>(std::to_string(user_id)))
                                .set_payload_claim("role", jwt::basic_claim<json_traits>(std::to_string(role)))
                                .sign(jwt::algorithm::hs256{secret_key_});
        return token;
    }

    bool JWT::verifyJWT(const std::string &token, std::string &user_id, int &role)
    {
        try
        {
            auto decoder = jwt::decode<json_traits>(token);
            auto verifer = jwt::verify<json_traits>()
                               .allow_algorithm(jwt::algorithm::hs256{secret_key_})
                               .with_issuer("cloud-disk");
            verifer.verify(decoder);
            user_id = decoder.get_payload_claim("user_id").as_integer();
            role = decoder.get_payload_claim("role").as_integer();
            return true;
        }
        catch (const std::exception &e)
        {
            return false;
        }
        
    }
}