
#include "user.srpc.h"
#include "workflow/WFFacilities.h"
#include "UserController.hpp"
#include "UserDao.hpp"
#include "UserService.hpp"
#include "Config.hpp"
#include "Logger.hpp"
using namespace srpc;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}


int main()
{
    // 检查版本号是否匹配
	GOOGLE_PROTOBUF_VERIFY_VERSION;
    // 读取配置
    std::string config_path = "conf/app.json";
    utils::Config config(config_path);
    
    // 初始化日志
    utils::Logger::init("log/user_services.log","info",0,0);
    LOG_INFO("user_services starting...");

	unsigned short port = 8081;
	SRPCServer server;

    dao::UserDao user_dao(config.mysql_url());
    services::UserService userservice(user_dao);
	controller::UserController userservice_impl(userservice);    
	server.add_service(&userservice_impl);

    if(server.start(port) == 0)
    {
        LOG_INFO("user_services start in port {}",port);
        wait_group.wait();
	    server.stop();
        google::protobuf::ShutdownProtobufLibrary();
        LOG_INFO("user_services stop success");
    }else{
        LOG_ERROR("user_services start failed");
        return -1;
    }

	
	return 0;
}
