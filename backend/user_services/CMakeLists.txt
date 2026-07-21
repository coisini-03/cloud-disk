set(USER_SERVICE_SRC
	src/main.cc
	src/UserDao.cc
	src/UserService.cc
	src/UserController.cc
)

add_executable(user_services ${USER_SERVICE_SRC})

target_include_directories(user_services
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(user_services
	PRIVATE
		clouddisk_common
		clouddisk_idl
)
