
add_executable(rabbitmq RabbitMQ.cc)

target_link_libraries(rabbitmq clouddisk_common SimpleAmqpClient)
