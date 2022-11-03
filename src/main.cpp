#include "echo.grpc.pb.h"
#include <asyncpp/grpc/rw_tag.h>
#include <asyncpp/grpc/server.h>
#include <asyncpp/grpc/task.h>
#include <asyncpp/timer.h>
#include <chrono>

using namespace asyncpp;
using namespace asyncpp::grpc;

task<&EchoService::AsyncService::RequestEcho> run_echo_task(EchoService::AsyncService* service, ::grpc::ServerCompletionQueue* cq) {
	run_echo_task(service, cq);

	auto& context = co_await rpc_info();
	std::cout << "Generating random hello for " << context.request().name() << std::endl;
	context.response().set_hello("Hello " + context.request().name());
	co_await std::chrono::seconds{1};
	std::cout << "... done " << std::endl;
	co_return ::grpc::Status::OK;
}

int main(int argc, const char** argv) {
	EchoService::AsyncService async_service;

	int port;
	// clang-format off
	auto s = server_builder()
		.set_thread_count(1)
        .add_port("0.0.0.0:50051", ::grpc::InsecureServerCredentials(), &port)
        .add_service(&async_service)
            .add_task(&run_echo_task)
        .build();
	// clang-format on

	std::cout << "Listening on 0.0.0.0:" << port << std::endl;
	for (std::string line; std::getline(std::cin, line);) {
		if (line == "q") break;
	}
	std::cout << "Shutting down" << std::endl;
}