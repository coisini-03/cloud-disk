#include <string>
#include <cstdint>
#include <iostream>
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

        friend std::ostream& operator<<(std::ostream& os, const SysUser& user) {
            os << "[SysUser]: " << user.user_name 
                << ", " << user.password_hash << ", " 
                << user.create_time << ", " 
                << "use_capacity: " << user.use_capacity << ", " 
                << "total_capacity: " << user.total_capacity << ", " 
                << "status: " << user.status << ", " 
                << "role: " << user.role << ", " 
                << "id: " << user.id;
            return os;
        }
    };

}