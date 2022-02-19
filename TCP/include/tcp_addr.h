#ifndef _TCP_ADDR_H
#define _TCP_ADDR_H

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

    std::string ip_;
    int port_;
};







#endif