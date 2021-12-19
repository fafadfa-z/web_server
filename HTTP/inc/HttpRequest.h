#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <sys/types.h>
#include <string>
#include <map>
#include <memory>

class TCPConnection;

using TCPConnectionPtr=std::shared_ptr<TCPConnection>;

namespace Http
{
    extern const char *RN;

    enum HttpMode
    {
        Invalid,
        Get,
        Post,
        Head,
        Put,
        Delete
    };
    enum HttpVersion
    {
        Unknown,
        Http10,
        Http11
    };

    class HttpRequest
    {
    public:
        HttpRequest()
            : request_(Invalid), version_(Unknown)
        {
        }

        bool readMessage(TCPConnectionPtr);

        void display();

        HttpMode request()const {return request_;}
        std::string query()const{return query_;}

    private:

        bool getRequest(char *, char *);
        void getHeader(char *, char *);


        bool setHttpMode(char *, char *);

        bool setHttpVersion(char *, char *);

        std::string displayMode();
        std::string displayVersion();

        std::map<std::string, std::string> headerMap_;

        std::string query_;
        HttpMode request_;
        HttpVersion version_;
    };

}
#endif