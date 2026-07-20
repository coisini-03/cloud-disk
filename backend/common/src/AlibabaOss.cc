#include "AlibabaOss.hpp"


using namespace AlibabaCloud::OSS;
namespace utils{
    AlibabaOss::AlibabaOss()
    { 
        InitializeSdk();
    }
    AlibabaOss::~AlibabaOss()
    {
        ShutdownSdk();
    }
    // 获取单例
    AlibabaOss& AlibabaOss::getInstance()
    {
        static AlibabaOss instance;
        return instance;
    }

    void AlibabaOss::init(const OssConfig &config){
        config_ = config;
        ClientConfiguration conf;
        conf.signatureVersion = SignatureVersionType::V4;
        /* 从环境变量中获取访问凭证。运行本代码示例之前，请确保已设置环境变量OSS_ACCESS_KEY_ID和OSS_ACCESS_KEY_SECRET。*/
        auto credentialsProvider = std::make_shared<EnvironmentVariableCredentialsProvider>();
        client_ = std::make_unique<OssClient>(config_.endpoint, config_.access_key_id, config_.access_key_secret, conf);
        client_->SetRegion(config_.region);
    }

    int AlibabaOss::putObject(const std::string& object_name, std::shared_ptr<std::iostream> content){
        PutObjectRequest request(config_.bucketname, object_name, content);
        auto outcome = client_->PutObject(request);
        if (!outcome.isSuccess()) {
            /* 异常处理。*/
            std::cout << "PutObject fail" <<
            ",code:" << outcome.error().Code() <<
            ",message:" << outcome.error().Message() <<
            ",requestId:" << outcome.error().RequestId() << std::endl;
            return -1;
        } 
        return 0;
    }
}