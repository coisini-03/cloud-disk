

add_subdirectory(dao)
add_subdirectory(services)
add_subdirectory(controller)


add_executable(clouddisk main.cc)

target_link_libraries(clouddisk PRIVATE
    clouddisk_common
    clouddisk_dao
    clouddisk_services
    clouddisk_controller
)
