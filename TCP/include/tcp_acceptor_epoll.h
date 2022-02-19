#ifndef _TCP_ACCEPTOR_EPOLL_H
#define _TCP_ACCEPTOR_EPOLL_H

#include <sys/epoll.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <functional>
#include <memory>
#include <errno.h>
#include <thread>
#include "tcp_addr.h"

#include "tcp_acceptor.h"

class EpollConnect:public Acceptor
{
public:
    EpollConnect(TCPAddr addr,std::function<void(int)>submitFun)
        :Acceptor(addr,submitFun)
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

    int nums_;
};

#endif
