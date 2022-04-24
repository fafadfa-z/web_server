#include <iostream>
#include <chrono>

#include "http_server.h"
#include "tcp_server.h"
#include "logger.h"
#include "local_message.h"
#include "timmer.h"


std::shared_ptr<TCPServer> server;

int main()
{
    std::cout << "hellow! " << std::endl;

    Base::LocalMassage::readConfig("/root/code/web_server/web_server/config.conf");

    Log::init();  // 初始化日志

    auto httpSever=Http::HttpServer::init(); // 初始化http服务器

    httpSever->beginServer();
    
    auto tim = Time::Timmer::init(10);

    tim->start(); // 阻塞调用，开启定时器

    return 0;
}