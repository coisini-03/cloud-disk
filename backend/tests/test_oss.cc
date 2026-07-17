#include <alibabacloud/oss/OssClient.h>
#include <utils/AlibabaOss.hpp>
#include <utils/Config.hpp>

using namespace AlibabaCloud::OSS;

using namespace utils;
int main(void)
{
    utils::Config config("/home/coisini/wangdao/code/cloud-disk/backend/conf/app.json");
    OssConfig oss_cfg;
    oss_cfg.endpoint = config.get<std::string>("oss.endpoint");
    oss_cfg.region = config.get<std::string>("oss.region");
    oss_cfg.bucketname = config.get<std::string>("oss.bucketName");
    oss_cfg.access_key_id = config.get<std::string>("oss.accessKeyId");
    oss_cfg.access_key_secret = config.get<std::string>("oss.accessKeySecret");
    utils::AlibabaOss::getInstance().init(oss_cfg);   
    int ret = utils::AlibabaOss::getInstance().putObject("test.txt", std::make_shared<std::stringstream>("hello world"));
    if(ret != 0){
        std::cout << "putObject fail" << std::endl;
    }   
    return 0;
}