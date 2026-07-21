

set(PROTO_SRC
    src/user.pb.cc
    src/UserClient.cc
    src/UserServer.cc
)

add_library(clouddisk_idl STATIC ${PROTO_SRC})

target_include_directories(clouddisk_idl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)