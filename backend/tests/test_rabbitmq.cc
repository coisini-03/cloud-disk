#include <utils/RabbitMQ.hpp>
#include <utils/Logger.hpp>
#include <utils/Config.hpp>
#include <string>

using namespace std;

void test_publish()
{
    utils::RabbitMQ::getInstance().publish("hellow world");
}

int main(int argc, char const *argv[])
{
 
    // 初始化配置
    utils::Config config("conf/app.json");
    // 初始化
    utils::Logger::init("/home/coisini/wangdao/code/cloud-disk/backend/log/app.log","info",0,0);
    // 初始化 RabbitMQ
    utils::RabbitMQConfig mq_config;
    mq_config.host = config.get<string>("rabbitmq.host");
    mq_config.port = config.get<int>("rabbitmq.port");
    mq_config.user = config.get<string>("rabbitmq.user");
    mq_config.password = config.get<string>("rabbitmq.password");
    mq_config.queue_name = config.get<string>("rabbitmq.queue_name");
    mq_config.exchange = config.get<string>("rabbitmq.exchange");
    mq_config.routing_key = config.get<string>("rabbitmq.routing_key");
    mq_config.vhost = config.get<string>("rabbitmq.vhost");
    utils::RabbitMQ::getInstance().init(mq_config);

    test_publish();
    return 0;
}

