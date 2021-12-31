#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <memory>
#include "htmlFile.h"

class TCPConnection;
class TCPServer;
class WebResources;

namespace Http
{
    class HttpServer
    {
    public:

        static HttpServer *init();

        void beginServer();

        std::shared_ptr<TCPServer> tcpServer()const {return tcpServer_;}

    private:
        HttpServer();

        void dealMessage(TCPConnection* conn);
        void dealConnection(TCPConnection* conn);
        char *findRequest(char *start, char *end);
        void connectClose(TCPConnection* conn);

        long connCount;  // 记录当前的连接数
 
        WebResources WebResources_;
        
        std::shared_ptr<TCPServer> tcpServer_;

        static HttpServer *entity;

        static const std::string sourcePath_;
    };

}
#endif