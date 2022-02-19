#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include <memory>
#include <vector>
#include "html_file.h"
#include "http_request.h"


class TCPConnection;
class TCPServer;
class WebResources;

namespace Http
{
    class HttpServer
    {
        using mapType= std::map<int,std::unique_ptr<HttpRequest>>;
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

        bool  handleMes(TCPConnection* conn,HttpRequest&);

    private:
        long connCount;  // 记录当前的连接数
 
        WebResources WebResources_;
        
        std::shared_ptr<TCPServer> tcpServer_;

        static HttpServer *entity;

        static const std::string sourcePath_;
        static thread_local mapType questMap_;
    };
}
#endif