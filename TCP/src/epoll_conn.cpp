
#include "epoll_conn.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include "timePoint.h"
#include "logger.h"
#include "channel.h"
#include "buffer.h"


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
            LOG_DEBUG << "more than one player.. " << messageSize << log::end;
        }
        else

            for (int i = 0; i < messageSize; i++)
            {
                if (events_[i].data.fd == listenFd_)  //如果是新连接
                {
                    acceptNewConn();

                    events_[i].data.fd = 0;
                    events_[i].events = 0;
                }
                else  
                {
                    
                    MapType::iterator temp;

                    temp = channelMap_.find(events_[i].data.fd);

                    assert(temp != channelMap_.end());
                    temp->second->dealEvent(events_[i]);
                }
            }
    }
    else if (messageSize == 0)
    {
        // LOG_INFO << "no player join..." << log::end;
    }
    else
    {
        LOG_FATAL << "errno: " << strerror(errno) << log::end;
    }
}
void EpollConnect::acceptNewConn()
{
    sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(sockaddr_in);

    auto connfd = accept(listenFd_, (sockaddr *)&clientAddress, &clientAddressLen);

    setSendBufSize(connfd, 2560); //设置发送缓存区大小

    char ipBuf[INET_ADDRSTRLEN];

    auto ip = inet_ntop(AF_INET, &clientAddress.sin_addr, ipBuf, INET_ADDRSTRLEN);
    auto port = ntohs(clientAddress.sin_port);

    nums_++;

    LOG_FATAL << "new player socket: " << connfd << " ip: " << ip << " port: " << port << " nums: " << nums_ << log::end;

    std::shared_ptr<Buffer>buf(std::make_shared<Buffer>());
    std::shared_ptr<Channel>channel(std::make_shared<Channel>(connfd,buf));
    
    channelMap_.insert({connfd,channel});

    epoll_event new_event;

    new_event.events = channel->events();
    
    new_event.data.fd =connfd;

    int old_option = fcntl(connfd, F_GETFL); //设置为非阻塞式
    int new_opion = old_option | O_NONBLOCK;
    fcntl(connfd, F_SETFL, new_opion);

    auto res = epoll_ctl(epollFd_, EPOLL_CTL_ADD, connfd, &new_event);

    assert(res == 0);


    newConnCallBack_(channel); //回调服务器类
}

void EpollConnect::enable_write(int fd)
{

    epoll_event new_event;

    new_event.data.fd =fd;

    new_event.events=EPOLLIN|EPOLLPRI|EPOLLOUT;
    
    auto res = epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &new_event);

    assert(res!=-1);
}

void EpollConnect::diseable_write(int fd)
{
    epoll_event new_event;

    new_event.data.fd =fd;

    new_event.events=EPOLLIN|EPOLLPRI;
    
    auto res = epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &new_event);

    assert(res!=-1);
}

void EpollConnect::removeFd(int fd)          //这个函数是事件处理线程调用的。
{
    LOG_DEBUG << "removeFd: " << fd << log::end;

    auto iter = channelMap_.find(fd);

    assert(iter != channelMap_.end());

    channelMap_.erase(iter);

    auto ret=epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr);

    assert(ret!=-1);    

    ::close(fd);
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
