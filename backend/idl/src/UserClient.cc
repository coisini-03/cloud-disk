#include "user.srpc.h"
#include "workflow/WFFacilities.h"

using namespace srpc;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}

static void login_done(::cloud_disk::LoginResponse *response, srpc::RPCContext *context)
{
}

static void register_done(::cloud_disk::RegisterResponse *response, srpc::RPCContext *context)
{
}

static void getuser_done(::cloud_disk::GetUserResponse *response, srpc::RPCContext *context)
{
}

int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	const char *ip = "127.0.0.1";
	unsigned short port = 1412;

	::cloud_disk::UserService::SRPCClient client(ip, port);

	// example for RPC method call
	::cloud_disk::LoginRequest login_req;
	//login_req.set_message("Hello, srpc!");
	client.Login(&login_req, login_done);

	wait_group.wait();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
