#include <alibabacloud/oss/OssClient.h>
using namespace AlibabaCloud::OSS;

int main(void)
{
    /* 初始化OSS账号信息。*/
            
    /* yourEndpoint填写Bucket所在地域对应的Endpoint。以华东1（杭州）为例，Endpoint填写为https://oss-cn-hangzhou.aliyuncs.com。*/
    std::string Endpoint = "https://oss-cn-wuhan-lr.aliyuncs.com";
    /* yourRegion填写Bucket所在地域对应的Region。以华东1（杭州）为例，Region填写为cn-hangzhou。 */
    std::string Region = "cn-wuhan-lr";
    /* 填写Bucket名称，例如examplebucket。*/
    std::string BucketName = "cloud-disk72";
    // 认证id
    std::string AccessKeyId = "LTAI5tAFoJD78XdV16Z2uFoQ";
    // 认证密钥
    std::string AccessKeySecret = "k0qXH05RomGrQE05aGGsKicDkWdTbf";
    /* 填写Object完整路径，完整路径中不能包含Bucket名称，例如exampledir/exampleobject.txt。*/
    std::string ObjectName = "exampledir/exampleobject.txt";

    /* 初始化网络等资源。*/
    InitializeSdk();

    ClientConfiguration conf;
    conf.signatureVersion = SignatureVersionType::V4;
    /* 从环境变量中获取访问凭证。运行本代码示例之前，请确保已设置环境变量OSS_ACCESS_KEY_ID和OSS_ACCESS_KEY_SECRET。*/
    auto credentialsProvider = std::make_shared<EnvironmentVariableCredentialsProvider>();
    OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
    client.SetRegion(Region);
    
    std::shared_ptr<std::iostream> content = std::make_shared<std::stringstream>();
    *content << "Thank you for using Alibaba Cloud Object Storage Service!";
    PutObjectRequest request(BucketName, ObjectName, content);

    /*（可选）请参见如下示例设置访问权限ACL为私有（private）以及存储类型为标准存储（Standard）。*/
    //request.MetaData().addHeader("x-oss-object-acl", "private");
    //request.MetaData().addHeader("x-oss-storage-class", "Standard");

    /* 上传文件。*/
    auto outcome = client.PutObject(request);

    if (!outcome.isSuccess()) {
        /* 异常处理。*/
        std::cout << "PutObject fail" <<
        ",code:" << outcome.error().Code() <<
        ",message:" << outcome.error().Message() <<
        ",requestId:" << outcome.error().RequestId() << std::endl;
        return -1;
    }

    /* 释放网络等资源。*/
    ShutdownSdk();
    return 0;
}