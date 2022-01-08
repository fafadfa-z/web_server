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


#include <mysql.h>
#include "mysqlConn.h"
#include "timePoint.h"

using std::string;

std::shared_ptr<TCPServer> server;


int main()
{
    std::cout << "hellow! " << std::endl;

    Log::init();

    auto httpSever=Http::HttpServer::init();

    httpSever->beginServer();
     
    // MySql_::SqlMess conn;

    //auto ret=conn.searchByName("???1");

    // auto ret=conn.insert("wulan","wulan",Time::getNowTime());


    while(1)
    {   
        sleep(10);
    }
    
    return 0;
}