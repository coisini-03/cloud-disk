#include <string>
#include <cstdint>

namespace models
{

    struct SysUser
    {
        std::string user_name;
        std::string password_hash;
        std::string create_time;
        std::uint64_t use_capacity;
        std::uint64_t total_capacity;
        int status;
        int role;
        int id;
    };

}