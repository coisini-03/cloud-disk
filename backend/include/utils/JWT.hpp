#include <string>

namespace utils
{
    class JWT
    {
    private:
        std::string secret_key_;
    public:
        JWT(std::string secret_key);
        ~JWT();
        std::string generateJWT(int &user_id,int role,int expiretime_s);
        bool verifyJWT(const std::string &token,std::string &user_id,int &role);
    };

}
