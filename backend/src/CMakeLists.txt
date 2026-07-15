
add_subdirectory(utils)

add_executable(clouddisk main.cc)

target_link_libraries(clouddisk PRIVATE
    clouddisk_utils
    clouddisk_common
)
