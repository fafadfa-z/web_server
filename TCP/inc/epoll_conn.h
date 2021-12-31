#ifndef CONNECT_H
#define CONNECT_H

#include <sys/epoll.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <functional>
#include <memory>
#include <errno.h>
#include <thread>
#include "tcpAddr.h"

#include "acceptor.h"

class EpollConnect:public acceptor
{
public:
    EpollConnect(TCPAddr addr,std::function<void(int)>submitFun)
        :acceptor(addr,submitFun)
    {
    }
    ~EpollConnect();
  
private:
    void setSendBufSize(int fd, size_t size = 5120)
    {
        ::setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
    }

    bool acceptInit() override;

    void acceptLoop() override;

    void addfd(int fd, unsigned short event); //向epoll 中添加文件描述符

private:

    int epollFd_; //epoll 复用

    static const int timeout_=100;

    static const int evenListMax_ = 16;

    epoll_event events_[evenListMax_];

    int nums;
};

#endif
