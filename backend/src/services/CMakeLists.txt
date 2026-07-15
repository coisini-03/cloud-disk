
set(SERVICE_SRC
    UserService.cc
)

add_library(clouddisk_services STATIC ${SERVICE_SRC})
target_link_libraries(clouddisk_services PRIVATE clouddisk_dao clouddisk_common clouddisk_utils)
