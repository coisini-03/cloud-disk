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

using namespace std;
WFFacilities::WaitGroup waitGroup(1);

int main(int argc,char *arg[]){

    // 信号处理函数
    signal(SIGINT, [](int){
        std::cout << "SIGINT signal received, server will stop" << std::endl;
        waitGroup.done();
    });

    wfrest::BluePrint auth_bp;
    wfrest::BluePrint file_bp;
    std::string db_url = "mysql://root:123456@localhost:3306/clouddisk";

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
    if(server.start(8080) == 0){
        std::cout << "Server started on port 8080" << std::endl;
        waitGroup.wait();
        server.stop();
    }else{
        std::cout << "Server start failed" << std::endl;
    }
    return 0;
}