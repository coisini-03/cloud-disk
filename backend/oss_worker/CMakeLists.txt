
set(OSS_WORKER_SRC 
    src/main.cc
)
add_executable(oss_worker ${OSS_WORKER_SRC})


target_link_libraries(oss_worker PRIVATE clouddisk_common)
