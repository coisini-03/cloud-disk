#include <utils/PreSql.hpp>

namespace utils
{
    PreSql::PreSql() = default;
    PreSql::~PreSql() = default;
    std::string PreSql::escape_sql(const std::string &in_sql)
    {
        std::string out_sql;
        for (char c : in_sql)
        {
            switch (c)
            {
            case '\'':
                out_sql += "\\'";
                break;
            case '\"':
                out_sql += "\\\"";
                break;
            case '\\':
                out_sql += "\\\\";
                break;
            case '\n':
                out_sql += "\\n";
                break;
            case '\r':
                out_sql += "\\r";
                break;
            case '\0':
                out_sql += "\\0";
                break;
            default:
                out_sql += c;
                break;
            }
        }
        return out_sql;
    }
}
