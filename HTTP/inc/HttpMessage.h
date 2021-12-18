#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#include <string>
#include <map>

#include <HttpRequest.h>



namespace Http
{
    enum HttpReplyMode
    {
        Http200 = 200, // 请求成功，信息在返回的相应报文中
        Http301 = 301, // 请求的对象已经永久转移了，新的URL定义在相应报文中的 Location: 首部行中
        Http400 = 400, // 400  表示这个请求不能被服务器理解。
        Http404 = 404, // 被请求的报文不在服务器上
        Http505 = 505  // 服务器不支持强求报文使用的HTTP格式。
    };

    enum HttpConnMode // 设置长连接还是短连接
    {
        ConnKeep,
        ConnClose
    };

    class HttpMessage
    {
    public:
        HttpMessage(HttpReplyMode mod = Http400, HttpVersion version = Http11)
            : mod_(mod), version_(version)
        {
        }

        void setMessageMode(HttpReplyMode mod) { mod_ = mod; }

        std::string dealMessage(const std::string &mes, const std::string &message);

        void fillRequestMessage(size_t size)
        {
            setContentLength(size);
            addHeader("Server", "fafadfa");
            addHeader("Content-Type", "text/html");
        }

        void addHeader(const std::string &title, const std::string &body);

    private:
        void setContentLength(size_t size)
        {
            addHeader("Content-Length", std::to_string(size));
        }

        std::string HeaderInfo_; //请求行的信息

        HttpReplyMode mod_;
        HttpVersion version_;
    };

}

#endif
