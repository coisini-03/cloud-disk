#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include "AlibabaOss.hpp" 
#include "RabbitMQ.hpp"  
#include "Logger.hpp" 

namespace services {
void oss_consumer_loop(utils::RabbitMQConfig config) {
    try {
        // 创建 Channel
        AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::CreateFromUri(config.get_uri());
        channel->DeclareQueue(config.queue_name, false, true, false, false);
        // 订阅队列
        std::string consumer_tag = channel->BasicConsume(config.queue_name);
        LOG_INFO("OSS 容灾备份后台线程启动，正在监听队列: {}", config.queue_name);

        while (true) {
            AmqpClient::Envelope::ptr_t envelope;

            bool got_msg = channel->BasicConsumeMessage(consumer_tag, envelope);
            LOG_INFO("OSS 容灾备份后台线程收到消息: {}", got_msg ? "是" : "否");
            if (got_msg) {
                try {
                    // 解析 JSON 消息
                    std::string payload = envelope->Message()->Body();
                    auto mq_msg = nlohmann::json::parse(payload);
                    
                    std::string physical_path = mq_msg["physical_path"];
                    std::string oss_path = std::to_string(mq_msg["uid"].get<int>()) + "/" + mq_msg["filename"].get<std::string>();

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
}
} // namespace services