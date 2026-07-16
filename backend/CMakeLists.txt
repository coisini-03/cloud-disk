
cmake_minimum_required(VERSION 3.22)

project(cloud-disk)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译后目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 加速编译选项
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "启用 ccache: ${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
else()
    message(WARNING "未找到 ccache 编译器，将使用默认编译器")
endif()

find_program(LLD_PROGRAM ld.lld)
if(LLD_PROGRAM)
    message(STATUS "启用 lld 链接器: ${LLD_PROGRAM}")
    add_link_options("-fuse-ld=lld")
else()
    message(WARNING "未找到 lld 链接器，将使用默认链接器")
endif()

# 公共接口库
# find_package(workflow REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(jwt-cpp REQUIRED)
find_package(spdlog REQUIRED)
find_package(wfrest REQUIRED)
add_library(clouddisk_common INTERFACE)

link_directories(/usr/local/lib)
target_include_directories(clouddisk_common INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(clouddisk_common INTERFACE
    workflow
    fmt
    jwt-cpp::jwt-cpp
    nlohmann_json
    sodium
    spdlog
    wfrest
)

add_subdirectory(src)
add_subdirectory(tests)

file(COPY ${CMAKE_SOURCE_DIR}/conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY ${CMAKE_SOURCE_DIR}/../frontend DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
