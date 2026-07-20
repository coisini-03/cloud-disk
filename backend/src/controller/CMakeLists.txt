set(CONTROLLER_SRC
	UserController.cc
	TlbFileController.cc
)
add_library(clouddisk_controller STATIC ${CONTROLLER_SRC})

target_link_libraries(clouddisk_controller
	PRIVATE
		clouddisk_services
		clouddisk_dao
		clouddisk_common
)

