#pragma once
#include <string>
#include <mutex>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
using namespace AmqpClient;
namespace utils
{
    struct RabbitMQConfig
    {
        std::string host;
        int port;
        std::string user;
        std::string password;
        std::string queue_name;
        std::string exchange;
        std::string routing_key;
        std::string vhost;

        std::string get_uri();
    };
    
    class RabbitMQ
    {
    private:
        ~RabbitMQ()=default;
        RabbitMQ()=default;
        RabbitMQConfig config_;
        Channel::ptr_t channel_;
        std::mutex mutex_;
    public:
        static RabbitMQ& getInstance();
        bool init(const RabbitMQConfig &config);
        bool publish(const std::string &message);
        RabbitMQ(const RabbitMQ &rhs)=delete;
        RabbitMQ& operator=(const RabbitMQ &rhs)=delete;
    };
}