set(FILE_SERVICE_SRC
	src/main.cc
	src/TlbFileDao.cc
	src/TlbFileService.cc
	src/TlbFileController.cc
)

add_executable(file_services ${FILE_SERVICE_SRC})

target_include_directories(file_services PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(file_services
	PRIVATE clouddisk_common
)

