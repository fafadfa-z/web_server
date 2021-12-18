#ifndef TCP_ADDR_H
#define TCP_ADDR_H

#include <string>
#include <assert.h>


class TCPAddr
{
public:
    TCPAddr(std::string ip,int port)
        :ip_(ip),port_(port)
    {
        assert(port_>0&&port_<=65536);

    }

    std::string ip()const {return ip_;}


    int port()const {return port_;}

private:

    int port_;
    std::string ip_;
};







#endif