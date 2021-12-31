#include <iostream>

#include <string>
#include <unistd.h>
#include <vector>

#include "server.h"
#include "logger.h"
#include "HttpServer.h"

#include "buffer.h"
#include "buff_pool.h"
#include "LocalMessage.h"

using std::string;

std::shared_ptr<TCPServer> server;


int main()
{
    std::cout << "hellow! " << std::endl;

    log::init();

    auto httpSever=Http::HttpServer::init();

    httpSever->beginServer();

    while(1)
    {   
        sleep(10);
    }
    
    return 0;
}