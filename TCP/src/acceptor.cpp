#include "acceptor.h"
#include <exception>
#include "logger.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "channel.h"

acceptor::acceptor(TCPAddr addr,TaskFun newConn,TaskFun readable)
    :addr_(addr),newConnCallBack_(newConn),readableCallBack_(readable),nums_(0)
{   

}

bool acceptor::begin(int fd)
{
    listenFd_=fd;

    Channel::setAcceptor(shared_from_this());

    auto ret=acceptInit();

    assert(ret);

    acceptThread_=std::thread(&acceptor::Loop,this);

    return true;
}

void acceptor::Loop()
{
    looping_=true;
    
    LOG_DEBUG << "acceptor listen begin......." << log::end;

    while(looping_)
    {
        acceptLoop();
    }
    
    LOG_DEBUG << "acceptor listen end......." << log::end;
}
int  acceptor::createListenSocket()
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

    LOG_DEBUG << "creeate socket success！" << log::end;

    return soc;
}


acceptor::~acceptor()
{
    acceptThread_.join();

    ::close(listenFd_);

    LOG_DEBUG << "虚基类析构"<< log::end;
}

