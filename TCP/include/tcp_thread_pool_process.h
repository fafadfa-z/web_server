#ifndef _TCP_THREAD_POOL_PROCESS_H
#define _TCP_THREAD_POOL_PROCESS_H

#include <map>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>

#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include "logger.h"
#include <sys/eventfd.h>
#include <sys/epoll.h>

#include "tcp_buff_pool.h"

class TCPServer;
class Channel;

#define NEW_PLAYER 1

class FdPack //为了防止管道粘包，封包和拆宝�?
{
    FdPack(int fd = -1)
    {
        buf_[0] = 88480;
        buf_[1] = fd;
        buf_[2] = 44840;
    }

    void sendFd()
    {
        if (buf_[1] != -1)
        {
            auto n = ::write(weakUpFd_, &buf_, size_);
            assert(n == size_);
        }
    }

    int getFd()
    {
        int temp[size_] = {0};

        auto n = ::read(weakUpFd_, &temp, size_);
        assert(n == size_);
    }
    static void setWeakUpFd(int fd) { weakUpFd_ = fd; }

private:
    static int weakUpFd_;
    static const int size_ = 3;

    int buf_[size_];
};

class PoolProcess : public std::enable_shared_from_this<PoolProcess>
{
public:
    using MapType = std::map<int, std::shared_ptr<Channel>>;

    PoolProcess();

    ~PoolProcess();

    void operator()();

    void pushConnect(const int fd)
    {
        LOG_INFO << "PoolProcess::pushConnect" << fd << "   epollfd:" << epollFd_ << Log::end;
        weakup(fd);
    }

    void changeEvent(int event, int fd);

    int weakUpFd() const { return pipe_[0]; }

    friend Channel;

private:
    void distribute();

    void removeFd(int fd);

    void weakup(int);

    void receiveWeakup();

    void insertToEpoll(const std::shared_ptr<Channel> &Channel);

private:
    static const int evenListMax_ = 16;

    int epollFd_;

    int pipe_[2];

    std::shared_ptr<Channel> weakupChannel_;

    MapType channelMap_;

    epoll_event events_[evenListMax_];

    const int timeout_;

    BuffPoolPtr bufPool_;

    inline static int triggerMod_;
};

#endif