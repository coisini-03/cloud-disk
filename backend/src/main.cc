#include <iostream>
#include <dao/UserDao.hpp>
#include <dao/TlbFileDao.hpp>
#include <services/TlbFileService.hpp>
#include <services/UserService.hpp>
#include <controller/UserController.hpp>
#include <controller/TlbFileController.hpp>
#include <wfrest/HttpServer.h>
#include <workflow/WFFacilities.h>
#include <csignal>
#include "Config.hpp"
#include "AlibabaOss.hpp"
#include "Logger.hpp"
#include "RabbitMQ.hpp"

using namespace std;
WFFacilities::WaitGroup waitGroup(1);

int main(int argc,char *arg[]){

    // 信号处理函数
    signal(SIGINT, [](int){
        std::cout << "SIGINT signal received, server will stop" << std::endl;
        waitGroup.done();
    });
    // 初始化项目配置
    utils::Config config("conf/app.json");
    // 初始化日志
    utils::Logger::init(
        config.get<std::string>("log.path", "app.log"),
        config.get<std::string>("log.level", "info"),
        config.get<int>("log.hour", 0),
        config.get<int>("log.minute", 0)
    );

    LOG_INFO("初始化日志完成");
    
    // 初始化OSS配置
    utils::OssConfig oss_cfg;
    oss_cfg.endpoint = config.get<std::string>("oss.endpoint");
    oss_cfg.region = config.get<std::string>("oss.region");
    oss_cfg.bucketname = config.get<std::string>("oss.bucketName");
    oss_cfg.access_key_id = config.get<std::string>("oss.accessKeyId");
    oss_cfg.access_key_secret = config.get<std::string>("oss.accessKeySecret");
    // 初始化OSS客户端
    utils::AlibabaOss::getInstance().init(oss_cfg);
    LOG_INFO("初始化OSS客户端");

    // 初始化rabbitmq配置
    utils::RabbitMQConfig rabbitmq_cfg;
    rabbitmq_cfg.host = config.get<std::string>("rabbitmq.host");
    rabbitmq_cfg.port = config.get<int>("rabbitmq.port");
    rabbitmq_cfg.user = config.get<std::string>("rabbitmq.user");
    rabbitmq_cfg.password = config.get<std::string>("rabbitmq.password");
    rabbitmq_cfg.queue_name = config.get<std::string>("rabbitmq.queue_name");
    rabbitmq_cfg.exchange = config.get<std::string>("rabbitmq.exchange");
    rabbitmq_cfg.routing_key = config.get<std::string>("rabbitmq.routing_key");
    rabbitmq_cfg.vhost = config.get<std::string>("rabbitmq.vhost");
    // 初始化rabbitmq客户端
    utils::RabbitMQ::getInstance().init(rabbitmq_cfg);
    LOG_INFO("初始化rabbitmq生产者成功");


    wfrest::BluePrint auth_bp;
    wfrest::BluePrint file_bp;
    std::string db_url = config.mysql_url();

    dao::UserDao userDao(db_url);
    services::UserService userService(userDao);
    controller::UserController userController(userService);

 
    dao::TlbFileDao fileDao(db_url);
    services::TlbFileService tlbFileService(fileDao);
    controller::TlbFileController tlbFileController(tlbFileService);
    
    // 创建服务器
    wfrest::HttpServer server;
    // 注册静态资源
    server.Static("/","../frontend/www/index.html");
    server.Static("/static","../frontend/www/static");
    // 注册路由
    userController.registerRoutes(auth_bp);
    server.register_blueprint(auth_bp,"/api/v1");
    tlbFileController.registerRoutes(file_bp);
    server.register_blueprint(file_bp,"/api/v1");
    LOG_INFO("注册路由完成");
    // 启动服务器
    int port = config.get<int>("server.port");
    if(server.start(port) == 0){
        LOG_INFO("Server started on port {}",port);
        waitGroup.wait();
        server.stop();
        LOG_INFO("Server stopped");
    }else{
        LOG_ERROR("Server start failed");
    }
    return 0;
}