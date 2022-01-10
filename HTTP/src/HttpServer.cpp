#include "HttpServer.h"
#include "server.h"
#include "algorithm"
#include "logger.h"
#include "HttpMessage.h"
#include "assert.h"
#include "HttpDeal.h"

class TCPConnection;

namespace Http
{
    thread_local  std::map<int,std::unique_ptr<HttpRequest>> HttpServer::questMap_;

    const std::string HttpServer::sourcePath_ =
        "/home/fafadfa/project/webserver/web_server/HtmlFiles";

    HttpServer *HttpServer::entity = nullptr;

    void send_to_baidu(const std::vector<char> &buf, int fd)
    {
        const char *ip = "14.215.177.38";
        int port = 80;

        struct sockaddr_in server_address;
        bzero(&server_address, sizeof(server_address));
        server_address.sin_family = AF_INET;
        inet_pton(AF_INET, ip, &server_address.sin_addr);
        server_address.sin_port = htons(port);

        int sockfd = socket(PF_INET, SOCK_STREAM, 0);
        //assert( sockfd >= 0 );
        if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        {
            printf("connection failed\n");
        }
        else
        {
            printf("send oob data out\n");

            send(sockfd, &buf[0], buf.size(), 0);

            char buffer[2048] = {0};

            auto size = ::recv(sockfd, buffer, 2048, 0);

            std::cout << "receive: \n"
                      << buffer << std::endl;

            send(fd, &buffer[0], size, 0);
        }

        close(sockfd);
    }

    HttpServer *HttpServer::init()
    {
        if (entity == nullptr)
        {
            entity = new HttpServer();
        }
        return entity;
    }

    HttpServer::HttpServer()
        : connCount(0), WebResources_(sourcePath_)
    {
        auto address = std::make_shared<TCPAddr>("192.168.1.100", 8848);

        tcpServer_ = TCPServer::init(address, 1);

        tcpServer_->setReadableCallBack(std::bind(&HttpServer::dealMessage, *this, std::placeholders::_1));
        tcpServer_->setConnectCallBack([this](TCPConnectionPtr conn) -> void
                                       { dealConnection(conn); });

        HttpDeal::setResource(&WebResources_);
    }

    void HttpServer::beginServer()
    {
        tcpServer_->beginServer();
    }
    
    bool  HttpServer::handleMes(TCPConnection* conn,HttpRequest&quest)
    {
        auto [status, index] = quest.readMessage(conn);  //读取信息

        if(status==CanDeal)  //数据可以处理
        {
            conn->read(index);

            HttpDeal deal(quest, *conn);

            deal.dealQuest(); //
            return true;
        }
        else if(status==MoreMes)  //数据没有完整接收，等待接收。
        {
            conn->read(index);
            return false;
        }
        else if(status==badMes)   //无法识别的数据
        {
            conn->read(index);
            conn->send("HTTP/1.1 400 BadRequest\r\n\r\n");
            return true;
        }
        else  LOG_FATAL << "意外的状态.." << Log::end;

        return true;
    }
    void HttpServer::dealMessage(TCPConnection *conn)
    {
        LOG_HTTP << "Begin dealMessage..." << Log::end;

        auto [left,right]=conn->buffer();
        std::string temp(left,right);

        std::cout<<std::endl<<"receive size: "<<temp.size()<<std::endl;
        std::cout<<temp;

        auto itor=questMap_.find(conn->fd());

        if(itor==questMap_.end()) //如果这个请求是第一次处理
        {
            LOG_INFO<<"New  player!"<<Log::end;

            std::unique_ptr<HttpRequest> quest=std::make_unique<HttpRequest>();

            auto ret=handleMes(conn,*quest);
            
            if(ret==false) questMap_.insert({conn->fd(),std::move(quest)});
        }
        else
        {
            LOG_DEBUG<<"Old player..."<<Log::end;

            auto& [fd,quest] =*itor;

            auto ret=handleMes(conn,*quest);

            if(ret==true) questMap_.erase(itor); 
        }
    }

    void HttpServer::connectClose(TCPConnection *conn)
    {

        LOG_HTTP << "HttpServer::connectClose" << Log::end;

        conn->send("HTTP/1.1 400 BadRequest\r\n\r\n");

        //conn->close();
    }

    void HttpServer::dealConnection(TCPConnection *conn)
    {
        connCount++;
    }

}