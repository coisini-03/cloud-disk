#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include "AlibabaOss.hpp" 
#include "RabbitMQ.hpp"  
#include "Logger.hpp" 
#include "Config.hpp" 
   

int main(int argc, char const *argv[])
{
    // 初始化项目配置
    utils::Config config("conf/app.json");
    // 初始化日志
    utils::Logger::init("log/oss_worker.log", "info", 0, 0);
    LOG_INFO("OSS 异步备份微服务正在启动...");

    // 初始化oss配置
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
    LOG_INFO("初始化rabbitmq消费者进程");

    try {
        // 创建 Channel
        AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::CreateFromUri(rabbitmq_cfg.get_uri());
        channel->DeclareQueue(rabbitmq_cfg.queue_name, false, true, false, false);
        // 订阅队列
        std::string consumer_tag = channel->BasicConsume(rabbitmq_cfg.queue_name);
        LOG_INFO("OSS 容灾备份后台线程启动，正在监听队列: {}", rabbitmq_cfg.queue_name);

        while (true) {
            AmqpClient::Envelope::ptr_t envelope;

            bool got_msg = channel->BasicConsumeMessage(consumer_tag, envelope);
            LOG_INFO("OSS 容灾备份后台进程收到消息: {}", got_msg ? "是" : "否");
            if (got_msg) {
                try {
                    // 解析 JSON 消息
                    std::string payload = envelope->Message()->Body();
                    auto mq_msg = nlohmann::json::parse(payload);
                    
                    std::string physical_path = mq_msg["physical_path"];
                    std::string oss_path = std::to_string(mq_msg["uid"].get<int>()) + "/" + mq_msg["filename"].get<std::string>();
                    LOG_INFO("OSS 容灾备份后台进程收到消息，物理路径: {}, OSS 路径: {}", physical_path, oss_path);
                    
                    // 读本地文件
                    std::ifstream ifs(physical_path, std::ios::binary);
                    if (!ifs) {
                        LOG_WARN("本地文件读取失败，可能已被删除: {}", physical_path);
                        continue;
                    }

                    auto stream_content = std::make_shared<std::stringstream>();
                    *stream_content << ifs.rdbuf();
                    ifs.close();

                    // 上传文件到 OSS
                    int ret = utils::AlibabaOss::getInstance().putObject(oss_path, stream_content);
                    
                    if (ret == 0) {
                        LOG_INFO("OSS 备份成功: {}", oss_path);
                    } else {
                        LOG_WARN("OSS 备份失败，等待重试: {}", oss_path);
                    }
                } catch (const nlohmann::json::exception& e) {
                    LOG_ERROR("JSON 解析失败，丢弃脏数据: {}", e.what());
                }
            }
        }
    } catch (const std::exception& e) {
        LOG_ERROR("消费者线程异常退出: {}", e.what());
    }
    
    return 0;
}


