#include "channel.h"
#include "poolprocess.h"
#include "logger.h"
#include "server.h"

#include <chrono>



thread_local std::shared_ptr<PoolProcess> Channel::poolPro_(nullptr);
TCPServer* Channel::server_=nullptr;

void Channel::setPoolPro(std::shared_ptr<PoolProcess> pro)
{
    poolPro_ = pro;
}
void Channel::setServer(TCPServer* server)
{
    server_=server;
}

void Channel::dealEvent(epoll_event &event)
{
    bool isDeal = false;

    if (event.events & POLLOUT)
    {
        LOG_INFO<<"re send begin..."<<Log::end;

        auto flag=buf_->sendSendable(fd_);

        if(flag) disableWrite();

         isDeal = true;
    }

    if ((event.events & POLLIN) || !buf_->empty()) //这个不能放在第一个，如果放在第一个则可能在写操作执行之前关闭端口
    {
        if (fd_==poolPro_->weakUpFd()) // weakup 情况
        {
            poolPro_->receiveWeakup();
        }
        else
        {
            assert(buf_!=nullptr);   
            
            auto res=buf_->saveReadable(fd_);

            if(res)
            {
                server_->connReadable(this);
            }
            else
            {
                Channel::close();
                //关闭channel
            }
        }
        isDeal = true;
    }
    if (event.events & (EPOLLHUP | POLLRDHUP))
    {
        LOG_FATAL << "client want to leave...." << Log::end;
    }
    if (isDeal == false)
    {
        LOG_FATAL << "other message:  " << event.events << Log::end;
    }
    event.events = 0;
    event.data.fd = 0;
}

void Channel::enableWrite()
{
    LOG_INFO << "channel: " << fd_ << "  enable write..." << Log::end;
    assert(!(events_ & POLLOUT));

    events_ |= POLLOUT;

    poolPro_->changeEvent(events_, fd_);
}

void Channel::disableWrite()
{
    LOG_INFO << "channel: " << fd_ << "  disable write..." << Log::end;
    assert(events_ & POLLOUT);

    events_ &= 0b1111'1111'1011;

    poolPro_->changeEvent(events_, fd_);
}

void Channel::close()
{
    poolPro_->removeFd(fd_);
}


void Channel::send(std::string &message)   //
{
    buf_->sendMessage(std::move(message));

    enableWrite();
}

Channel::~Channel()
{
    LOG_DEBUG << "channel 析构" << fd_ << Log::end;
}
