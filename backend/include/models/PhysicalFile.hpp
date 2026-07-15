#include <string>
#include <cstdint>

namespace models{
    struct PhysicalFile
    {
        int id;
        std::string file_hash;
        std::uint64_t file_size;
        std::string storage_path;
        int ref_count;
        std::string last_time;
    };
}
