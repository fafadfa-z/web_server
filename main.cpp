#include <iostream>

#include "tcp_server.h"
#include "logger.h"

#include "http_server.h"



std::shared_ptr<TCPServer> server;

int main()
{
    std::cout << "hellow! " << std::endl;

    Log::init();

    auto httpSever=Http::HttpServer::init();

    httpSever->beginServer();
    

    while(1)
    {   
        sleep(10);
    }
    
    return 0;
}