#include <iostream>

#include "http_server.h"
#include "tcp_server.h"
#include "logger.h"
#include "local_message.h"

std::shared_ptr<TCPServer> server;

int main()
{
    std::cout << "hellow! " << std::endl;

    Base::LocalMassage::readConfig("/root/web_server/config.conf");

    Log::init();

    auto httpSever=Http::HttpServer::init();

    httpSever->beginServer();
    

    while(1)
    {   
        sleep(10);
    }
    
    return 0;
}