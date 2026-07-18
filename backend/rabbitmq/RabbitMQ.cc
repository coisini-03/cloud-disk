#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <string>
#include <iostream>
using namespace std;
using namespace AmqpClient;

void pull_message()
{
    // 1. 以 URI 的方式创建 Channel
    string uri = "amqp://guest:guest@localhost:5672/%2f";
    Channel::ptr_t channel = Channel::CreateFromUri(uri);

    // 2. 获取消息
    // 方式 1: 拉取模式 --- 消费者主动从队列中拉取消息（非阻塞式）
    // const string& q = "oss.queue";

    // 如果队列中有消息，将消息放入到 evelope 中，并返回 true
    // 如果队列中没有消息，BasicGet 会立刻返回 false
    // Envelope::ptr_t envelope;
    // channel->BasicGet(envelope, q);
    // if (envelope && envelope->Message()) { */
    //     cout << envelope->Message()->Body() << endl;
    // }

    // 方式 2: 推送模式 --- 等待 RabbitMQ 推送消息（阻塞式）
    const string& q = "upload";
    channel->BasicConsume(q); // 订阅队列
    // 阻塞: 等待 RabbitMQ 推送消息
    Envelope::ptr_t envelope = channel->BasicConsumeMessage();
    // 打印消息
    if (envelope && envelope->Message()) {
        cout << envelope->Message()->Body() << endl;
    }
}

int main()
{

    // 1. 创建Channel

    string host = "127.0.0.1";
    int port = 5672;
    string username = "guest";
    string password = "guest";
    string vhost = "/";
    Channel::ptr_t channel = Channel::Create(host, port, username, password, vhost);
    // 2. 构建消息
    BasicMessage::ptr_t message = BasicMessage::Create("Hello RabbitMQ");
    // 3. 发送消息
    string exchange = "clouddisk"; // 交换机
    string routingKey = "key"; // 消息的routingKey
    channel->BasicPublish(exchange, routingKey, message); 

    pull_message();
}