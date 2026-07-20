#pragma once    
#include <string>

namespace utils
{ 
    class PreSql {
    public:
        explicit PreSql();
        ~PreSql();
        static std::string escape_sql(const std::string& sql);
    };
}
