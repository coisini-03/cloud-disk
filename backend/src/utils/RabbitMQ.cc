#include <utils/RabbitMQ.hpp>
#include <utils/Logger.hpp>

namespace utils{
    std::string RabbitMQConfig::get_uri()
    {
        if (vhost == "/")
        {
            vhost = "%2f";
        }
        return "amqp://" + user + ":" + password + "@" + host + ":" + std::to_string(port) + "/" + vhost;
    }

    RabbitMQ& RabbitMQ::GetInstance()
    {
        static RabbitMQ instance;
        return instance;
    }
    bool RabbitMQ::init(const RabbitMQConfig &config)
    {
        config_ = config;
        try{
            channel_ = Channel::CreateFromUri(config_.get_uri());
            LOG_INFO("RabbitMQ init success");
            
        }catch(const std::exception &e){
            LOG_WARN("RabbitMQ init failed: {}", e.what());
            return false;
        }
        return true;
    }
    bool RabbitMQ::publish(const std::string &message)
    {
        // 加锁，确保线程安全
        std::lock_guard<std::mutex> lock(mutex_);
        try{
            channel_->BasicPublish(config_.exchange, config_.routing_key, BasicMessage::Create(message));
            LOG_INFO("RabbitMQ publish success");
            return true;
        }catch(const std::exception &e){
            LOG_WARN("RabbitMQ publish failed: {}", e.what());
            return false;
        }
    }
}