#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <sys/types.h>
#include <string>
#include <map>
#include <memory>

class TCPConnection;
namespace Http
{
    extern const char *RN;

    enum HttpMode
    {
        Invalid,
        Get,
        Post,
    };
    enum HttpVersion
    {
        Unknown,
        Http10,
        Http11
    };

    enum MesState
    {
        CanDeal,    //正常，来了一个消息
        MoreMes,    //正常，后面还有消息
        badMes     //错误，无法解析的消息

    };

    class HttpRequest
    {
    public:
        HttpRequest()
        :   request_(Invalid), 
            version_(Unknown),
            state_(empty_)
        {
        }

        std::pair<MesState,char*> readMessage(TCPConnection *);

        void display();

        HttpMode request() const { return request_; }
        const std::string& query() const { return query_; }
        const std::string& entity()const { return entity_;}

        void clear();

    private:
        bool getRequest(char *, char *);
        bool setHttpMode(const char *, const char *);
        bool setHttpVersion(char *, char *);

        char* getHeader(char *, char *);
        char* findMessageHead(TCPConnection*);

        std::string displayMode();
        std::string displayVersion();

    private:

        enum State
        {
            empty_,
            needHead_,
            needEntity_,
            finish_
        };

    private:


        std::map<std::string, std::string> headerMap_; //请求行

        std::string query_;  //URL
        
        std::string entity_; //实体部分

        HttpMode request_;

        HttpVersion version_;

        static const size_t maxRequestSize=2048; //请求行的最大长度

        State state_;

    };

}
#endif