
#include "tcp_acceptor_epoll.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>

#include "time_point.h"
#include "logger.h"


bool EpollConnect::acceptInit()
{
    epollFd_ = epoll_create(5);

    assert(epollFd_ != -1);

    addfd(listenFd_, EPOLLIN);

    return true;
}

using namespace std::chrono;

void EpollConnect::acceptLoop()
{
    int messageSize = epoll_wait(epollFd_, events_, evenListMax_, timeout_);

    if (messageSize > 0)
    {
        if (messageSize > 1)
        {
            LOG_DEBUG << "more than one player.. " << messageSize << Log::end;
        }
        else
            for (int i = 0; i < messageSize; i++)
            {
                sockaddr_in clientAddress;
                socklen_t clientAddressLen = sizeof(sockaddr_in);

                auto connfd = accept(events_[i].data.fd, (sockaddr *)&clientAddress, &clientAddressLen);

                auto beginTime = std::chrono::system_clock::now(); //记录当前时间

                setSendBufSize(connfd, 2560); //设置发送缓存区大小

                events_[i].data.fd = 0;
                events_[i].events = 0;

                char ipBuf[INET_ADDRSTRLEN];

                auto ip = inet_ntop(AF_INET, &clientAddress.sin_addr, ipBuf, INET_ADDRSTRLEN);
                auto port = ntohs(clientAddress.sin_port);

                nums_++;

                LOG_FATAL << "new player socket: " << connfd << " ip: " << ip << " port: " << port << " nums: " << nums_ << Log::end;

                submitCallBack_(connfd,beginTime); //回调,提交描述符
            }
    }
    else if (messageSize == 0)
    {
        // LOG_INFO << "no player join..." << Log::end;
    }
    else
    {
        LOG_FATAL << "errno: " << strerror(errno) << Log::end;
    }
}

//添加事件
void EpollConnect::addfd(int fd, unsigned short signal)
{
    epoll_event event;

    event.data.fd = fd;
    event.events = signal;

    epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event);
}

EpollConnect::~EpollConnect()
{
    
    ::close(epollFd_);
}
