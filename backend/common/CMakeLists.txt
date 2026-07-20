

set(COMMON_SRC
	src/JWT.cc
	src/Hash.cc
	src/Logger.cc
	src/Config.cc
	src/PreSql.cc
	src/AlibabaOss.cc
	src/RabbitMQ.cc
)

add_library(clouddisk_common STATIC ${COMMON_SRC})

target_include_directories(clouddisk_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(clouddisk_common PUBLIC clouddisk_thirdparty)
