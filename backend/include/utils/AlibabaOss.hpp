#pragma once
#include <alibabacloud/oss/OssClient.h>
#include <memory>
#include <string>
namespace utils{
    struct OssConfig
    {
        std::string endpoint;
        std::string region;
        std::string bucketname;
        std::string access_key_id;
        std::string access_key_secret;
    };
    
    class AlibabaOss{
    public:
        static AlibabaOss& getInstance();
        void init(const OssConfig& config);
        int putObject(const std::string& object_name, std::shared_ptr<std::iostream> content);
    private:
        AlibabaOss();
        ~AlibabaOss();
        AlibabaOss& operator=(const AlibabaOss&) = delete;
        AlibabaOss(const AlibabaOss&) = delete;
        OssConfig config_;
        std::unique_ptr<AlibabaCloud::OSS::OssClient> client_;
    };
}