#ifndef HTTP_DEAL_H
#define HTTP_DEAL_H

#include <memory>
#include <map>

class TCPConnection;

namespace Http
{

    #define displaySendMess  0
    

    class HttpRequest;
    class WebResources;

    class HttpDeal
    {
    public:
        HttpDeal(HttpRequest& quest,TCPConnection& conn)
        :quest_(quest),conn_(conn)
        {}

        void dealQuest();

        static void setResource(WebResources*);

    private:
        HttpRequest&   quest_;
        
        TCPConnection& conn_;

        static WebResources* resource_;  //多线程在这里只做读取操作，应该不会出现资源竞争问题。
    };

}

#endif