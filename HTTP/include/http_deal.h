#ifndef _HTTP_DEAL_H
#define _HTTP_DEAL_H

#include <memory>
#include <map>

#include <assert.h>

class TCPConnection;
class HtmlFile;

namespace Http
{
    class HttpRequest;
    class WebResources;

    class HttpDeal
    {
    public:
        HttpDeal(HttpRequest& quest,TCPConnection& conn)
        :quest_(quest),conn_(conn)
        {}

        bool dealQuest();

        static void setResource(WebResources*);

    private:
        void sendBadMessage();

        void readEntity(); //��ȡHTTP���ĵ�ʵ����Ϣ

        void sendFile(const std::string&);  //����w

        bool addNewPlayer();  //处理系用户注册的信息。

    private:
        std::map<std::string,std::string> entityMap_;

        HttpRequest&   quest_;
        
        TCPConnection& conn_;

        static WebResources* resource_;  //多线程在这里�?做�?�取操作，应该不会出现资源竞争问题�?
    };

}

#endif