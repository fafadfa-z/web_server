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

    void HttpServer::dealMessage(TCPConnection *conn)
    {
        LOG_HTTP << "Begin dealMessage..." << Log::end;
        //send_to_baidu(buf,conn->fd());

        // auto [left,right]=conn->buffer();
        // std::string temp(left,right);

        // std::cout<<std::endl<<"receive size: "<<temp.size()<<std::endl;
        // std::cout<<temp;

        auto [status, index] = quest_.readMessage(conn);  //������Ϣ���ж���Ϣ��ʽ����ȷ��

        if(status==CanDeal)  //�յ�һ����������Ϣ
        {
            conn->read(index);

            HttpDeal deal(quest_, *conn);

            deal.dealQuest(); //������Ϣ
            quest_.clear();

        }
        else if(status==MoreMes)  //��ʽ��ȷ��������Ϣ����ȫ
        {
            conn->read(index);
        }
        else if(status==badMes)   //��ʽ����
        {
            quest_.clear();
            conn->read(index);
            conn->send("HTTP/1.1 400 BadRequest\r\n\r\n");

        }
        else  LOG_FATAL << "�����״̬������" << Log::end;

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