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
        CanDeal,    //����������һ����Ϣ
        MoreMes,    //���������滹����Ϣ
        badMes     //�����޷���������Ϣ

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


        std::map<std::string, std::string> headerMap_; //������

        std::string query_;  //URL
        
        std::string entity_; //ʵ�岿��

        HttpMode request_;

        HttpVersion version_;

        static const size_t maxRequestSize=2048; //�����е���󳤶�

        State state_;

    };

}
#endif