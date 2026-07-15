set(UTILS_SRC
	JWT.cc
)

add_library(clouddisk_utils STATIC ${UTILS_SRC})

target_link_libraries(clouddisk_utils PUBLIC clouddisk_common)
