#pragma once
#include <string>
#include <cstdint>

namespace models
{
    struct LogicalFile
    {
        int id;
        int user_id;
        int parent_id;
        std::string file_name;
        int is_folder;
        int del_falg;
        std::string last_time;
        int physical_file_id;
        std::uint64_t transfer_offset;
        int transfer_status;
    };
}