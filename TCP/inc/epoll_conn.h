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
    EpollConnect(TCPAddr addr,TaskFun newConn,TaskFun readable)
        :acceptor(addr,newConn,readable)
    {
    }
    ~EpollConnect();

public:


    void enable_write(int) override;  //开始写入

    void diseable_write(int)override; // 结束写入

    void removeFd(int)override; // 结束写入


private:

    bool acceptInit() override;

    void acceptLoop() override;

private:    
    
    void acceptNewConn();


    void addfd(int fd, unsigned short event); //向epoll 中添加文件描述符
    
    void setSendBufSize(int fd, size_t size = 5120)
    {
        ::setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
    }

  

    int epollFd_; //epoll 复用

    static const int timeout_=100;

    static const int evenListMax_ = 16;

    epoll_event events_[evenListMax_];


};

#endif
