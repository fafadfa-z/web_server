#include "channel.h"
#include "poolprocess.h"
#include "logger.h"
#include "server.h"

#include <chrono>

std::shared_ptr<acceptor> Channel::acceptor_(nullptr);

void Channel::setAcceptor(std::shared_ptr<acceptor> acc)
{
    acceptor_ = acc;
}

void Channel::dealEvent(epoll_event &event)
{
    bool isDeal = false;

    if (event.events & POLLOUT)
    {
        LOG_INFO << "re send begin..." << log::end;

        auto flag = buf_->sendSendable(fd_);

        if (flag)
            disableWrite();

        isDeal = true;
    }

    if (event.events & (POLLIN)) //这个不能放在第一个，如果放在第一个则可能在写操作执行之前关闭端口
    {
        assert(buf_ != nullptr);

        auto res = buf_->saveReadable(fd_);

        if (res)
        {
            acceptor_->readableCallBack_(shared_from_this());
        }
        else
        {
            Channel::close();
            //关闭channel
        }
        isDeal = true;
    }
    if (event.events & (EPOLLHUP | POLLRDHUP))
    {
        LOG_FATAL << "client want to leave...." << log::end;
    }
    if (isDeal == false)
    {
        LOG_FATAL << "other message:  " << event.events << log::end;
    }
    event.events = 0;
    event.data.fd = 0;
}

void Channel::enableWrite()
{
    LOG_INFO << "channel: " << fd_ << "  enable write..." << log::end;
    assert(!(events_ & POLLOUT));

    events_|=POLLOUT;

    acceptor_->enable_write(fd_);
}

void Channel::disableWrite()
{
    LOG_INFO << "channel: " << fd_ << "  disable write..." << log::end;
    assert(events_ & POLLOUT);

    events_=3;

    acceptor_->diseable_write(fd_);
}

void Channel::close()
{
    acceptor_->removeFd(fd_);
}

void Channel::readBuf(std::vector<char> &vec)
{
    buf_->readReadbale(vec);
}

void Channel::send(std::string &message) //
{
    buf_->sendMessage(std::move(message));

    enableWrite();
}

Channel::~Channel()
{
    LOG_DEBUG << "channel 析构" << fd_ << log::end;
}
