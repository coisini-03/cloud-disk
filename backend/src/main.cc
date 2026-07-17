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
#include <utils/Config.hpp>
#include <utils/AlibabaOss.hpp>

using namespace std;
WFFacilities::WaitGroup waitGroup(1);

int main(int argc,char *arg[]){

    // 信号处理函数
    signal(SIGINT, [](int){
        std::cout << "SIGINT signal received, server will stop" << std::endl;
        waitGroup.done();
    });

    // 初始化配置
    utils::Config config("conf/app.json");

        // 初始化OSS配置
    utils::OssConfig oss_cfg;
    oss_cfg.endpoint = config.get<std::string>("oss.endpoint");
    oss_cfg.region = config.get<std::string>("oss.region");
    oss_cfg.bucketname = config.get<std::string>("oss.bucketName");
    oss_cfg.access_key_id = config.get<std::string>("oss.accessKeyId");
    oss_cfg.access_key_secret = config.get<std::string>("oss.accessKeySecret");
    // 初始化OSS客户端
    utils::AlibabaOss::getInstance().init(oss_cfg);
    

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
    // 启动服务器
    int port = config.get<int>("server.port");
    if(server.start(port) == 0){
        std::cout << "Server started on port " << port << std::endl;
        waitGroup.wait();
        server.stop();
    }else{
        std::cout << "Server start failed" << std::endl;
    }
    return 0;
}