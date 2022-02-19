#include "tcp_acceptor.h"
#include <exception>
#include "logger.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

Acceptor::Acceptor(TCPAddr addr,std::function<void(int)>submit)
    :submitCallBack_(submit),
     addr_(addr)
{

}

bool Acceptor::begin(int fd)
{
    listenFd_=fd;

    auto ret=acceptInit();

    assert(ret);

    acceptThread_=std::thread(&Acceptor::Loop,this);

    return true;
}

void Acceptor::Loop()
{
    looping_=true;
    
    LOG_DEBUG << "acceptor listen begin......." << Log::end;

    while(looping_)
    {
        acceptLoop();
    }
    
    LOG_DEBUG << "acceptor listen end......." << Log::end;
}
int  Acceptor::createListenSocket()
{
    auto soc=socket(PF_INET, SOCK_STREAM, 0);

    assert(soc>=0);

    int temp = 1;
    ::setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp));

    sockaddr_in address;

    bzero(&address, sizeof(address));

    address.sin_family = AF_INET;

    address.sin_addr.s_addr = htonl(INADDR_ANY);

    address.sin_port = htons(addr_.port());

    auto res = bind(soc, (sockaddr *)&address, sizeof(address));

    assert(res != -1);

    res = ::listen(soc, 5);

    assert(res != -1);

    LOG_DEBUG << "creeate socket success！" << Log::end;

    return soc;
}


Acceptor::~Acceptor()
{
    acceptThread_.join();

    ::close(listenFd_);

    LOG_DEBUG << "虚基类析构"<< Log::end;
}

