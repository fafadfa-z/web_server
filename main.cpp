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
 #include  <filesystem>
int main()
{
    // std::cout << "hellow! " << std::endl;

    // Log::init();

    // auto httpSever=Http::HttpServer::init();

    // httpSever->beginServer();

    // MySql_::SqlMess conn;

    //auto ret=conn.searchByName("???1");

    // auto ret=conn.insert("wulan","wulan",Time::getNowTime());

   

    std::string pathStr="\\home\\fafadfa\\project\\webserver\\web_server\\HtmlFiles";

    std::cout << "pathStr: " << pathStr << std::endl;

    std::filesystem::directory_entry direct(pathStr);

    assert(direct.is_directory());

    for (const auto &entry : std::filesystem::recursive_directory_iterator(direct))
    {
        if (entry.is_regular_file())
        {
            // HtmlFile *file = new HtmlFile(entry.path()); //path 可以隐式转换成 string

            // //insertToMap(file, dp->d_name);

            // num_++;

            std::cout << "Find resource: " << entry << std::endl;
        }
    }

    while (1)
    {
        sleep(10);
    }

    return 0;
}