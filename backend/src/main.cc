#include <iostream>
#include <dao/UserDao.hpp>
#include <services/UserService.hpp>
#include <controller/UserController.hpp>
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

    wfrest::BluePrint bp;
    std::string db_url = "mysql://root:123456@localhost:3306/clouddisk";

    dao::UserDao userDao(db_url);
    services::UserService userService(userDao);
    controller::UserController userController(userService);

    
    // 创建服务器
    wfrest::HttpServer server;
    // 注册静态资源
    server.Static("/","../frontend/www/index.html");
    server.Static("/static","../frontend/www/static");
    // 注册路由
    userController.registerRoutes(bp);
    server.register_blueprint(bp,"/api/v1");
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