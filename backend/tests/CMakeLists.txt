
# 定义测试可执行文件
add_executable(test_jwt test_jwt.cc)
add_executable(test_hash test_hash.cc)
add_executable(test_logger test_logger.cc)
add_executable(test_config test_config.cc)
add_executable(test_insert test_insert.cc)

target_include_directories(test_jwt PRIVATE ${CMAKE_SOURCE_DIR}/include)
# 链接 GTest 库以及可能需要的线程库和加密库（如 OpenSSL）
target_link_libraries(test_jwt
    clouddisk_utils
    clouddisk_common
    # 如果 JWT 用了第三方库或需要链接主程序的源文件，也加在这里
)
target_link_libraries(test_hash
    clouddisk_utils
    clouddisk_common
)
target_link_libraries(test_logger
    clouddisk_utils
    clouddisk_common
)
target_link_libraries(test_config
    clouddisk_utils
    clouddisk_common
)
target_link_libraries(test_insert
    clouddisk_utils
    clouddisk_common
    clouddisk_dao
)

