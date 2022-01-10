#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

#include "server.h"
#include "logger.h"
#include "HttpServer.h"

#include "LocalMessage.h"

#include "timePoint.h"

#include <fstream>
using namespace std;

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