# 定义测试可执行文件
add_executable(test_jwt test_jwt.cc)
add_executable(test_hash test_hash.cc)
add_executable(test_logger test_logger.cc)
add_executable(test_config test_config.cc)
add_executable(test_user test_user.cc)
add_executable(test_userservice test_userservice.cc)
add_executable(test_tlbfiledao test_tlbfiledao.cc)
add_executable(test_oss test_oss.cc)
add_executable(test_tlbfileservice test_tlbfileservice.cc)
add_executable(test_rabbitmq test_rabbitmq.cc)

target_compile_options(test_oss PRIVATE -fno-rtti)
# 链接 GTest 库以及可能需要的线程库和加密库（如 OpenSSL）
target_link_libraries(test_jwt
	clouddisk_common
	# 如果 JWT 用了第三方库或需要链接主程序的源文件，也加在这里
)
target_link_libraries(test_hash
	clouddisk_common
)
target_link_libraries(test_logger

	clouddisk_common
)
target_link_libraries(test_config

	clouddisk_common
)
target_link_libraries(test_user

	clouddisk_common
	clouddisk_dao
)
target_link_libraries(test_userservice

	clouddisk_common
	clouddisk_dao
	clouddisk_services
)
target_link_libraries(test_tlbfiledao

	clouddisk_common
	clouddisk_dao
)
target_link_libraries(test_oss
	clouddisk_common
)
target_link_libraries(test_tlbfileservice
	clouddisk_common
	clouddisk_dao
	clouddisk_services
)
target_link_libraries(test_rabbitmq
	clouddisk_common
)
