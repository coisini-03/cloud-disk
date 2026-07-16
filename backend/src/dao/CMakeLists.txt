
set(DAO_SRC
    UserDao.cc
    TlbFileDao.cc
)

add_library(clouddisk_dao ${DAO_SRC})

target_link_libraries(clouddisk_dao PRIVATE clouddisk_common clouddisk_utils)
