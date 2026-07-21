#include "user.srpc.h"
#include "workflow/WFFacilities.h"

using namespace srpc;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}

class UserServiceServiceImpl : public ::cloud_disk::UserService::Service
{
public:

	void Login(::cloud_disk::LoginRequest *request, ::cloud_disk::LoginResponse *response, srpc::RPCContext *ctx) override
	{
		// TODO: fill server logic here
	}

	void Register(::cloud_disk::RegisterRequest *request, ::cloud_disk::RegisterResponse *response, srpc::RPCContext *ctx) override
	{
		// TODO: fill server logic here
	}

	void GetUser(::cloud_disk::GetUserRequest *request, ::cloud_disk::GetUserResponse *response, srpc::RPCContext *ctx) override
	{
		// TODO: fill server logic here
	}
};

int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	unsigned short port = 1412;
	SRPCServer server;

	UserServiceServiceImpl userservice_impl;
	server.add_service(&userservice_impl);

	server.start(port);
	wait_group.wait();
	server.stop();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
