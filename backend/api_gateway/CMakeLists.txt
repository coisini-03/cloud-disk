
set(API_GATEWAY_SRC
    src/main.cc
)

add_executable(api_gateway ${API_GATEWAY_SRC})

target_link_libraries(api_gateway
    clouddisk_common
    clouddisk_idl
)
