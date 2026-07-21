
#include <nlohmann/json.hpp>
#include <workflow/WFFacilities.h>

#include "user.srpc.h"
#include "UserController.hpp"
#include "UserDao.hpp"
#include "UserService.hpp"
#include "Config.hpp"
#include "Logger.hpp"

using namespace srpc;
using json = nlohmann::json;
static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}

void register_to_consul(const std::string& service_name, const std::string& service_ip, int port) {
    // 构建请求体
    std::string service_id = service_name + "-" + std::to_string(port);
    json req_data = {
        {"ID", service_id},
        {"Name", service_name},
        {"Address", service_ip},
        {"Port", port},
        {"Check", {
                {"TCP", service_ip + ":" + std::to_string(port)},
                {"Interval", "10s"},
                {"Timeout", "1s"},
                {"DeregisterCriticalServiceAfter", "30s"},
            }
        }
    };


    std::string consul_rul = "http://127.0.0.1:8500/v1/agent/service/register";
    std::string req_json = req_data.dump();
    // 创建http任务
    WFHttpTask *task = WFTaskFactory::create_http_task(consul_rul,0,2,[](WFHttpTask *task){
        if(task->get_state() != WFT_STATE_SUCCESS){
            LOG_ERROR("register_to_consul failed, error: {}",task->get_error());
            return;
        }
        LOG_INFO("register_to_consul success");
    });
    // 设置请求
    protocol::HttpRequest *req = task->get_req();
    req->set_method("PUT");
    req->set_header_pair("Content-Type", "application/json");
    req->append_output_body(req_json);

    task->start();
}
    
int main(int argc, char *argv[])
{

    // 检查版本号是否匹配
	GOOGLE_PROTOBUF_VERIFY_VERSION;
    // 读取配置
    std::string config_path = "conf/app.json";
    utils::Config config(config_path);
    
    // 初始化日志
    utils::Logger::init("log/user_services.log","info",0,0);
    LOG_INFO("user_services starting...");
    if(argc < 2){
        LOG_ERROR("user_services: port required");
        return -1;
    }

	unsigned short port = std::stoi(argv[1]);
	SRPCServer server;

    dao::UserDao user_dao(config.mysql_url());
    services::UserService userservice(user_dao);
	controller::UserController userservice_impl(userservice);    
	server.add_service(&userservice_impl);

    if(server.start(port) == 0)
    {
        LOG_INFO("user_services start in port {}",port);
        // 注册服务
        register_to_consul("user_services","192.168.83.128",port);
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
