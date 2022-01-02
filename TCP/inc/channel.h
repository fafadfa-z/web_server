#ifndef CHANNEL_H
#define CHANNEL_H

#include <memory>
#include <string>
#include <functional>
#include <mutex>

#include <sys/epoll.h>
#include <sys/poll.h>
#include <assert.h>
#include <vector>

#include "timePoint.h"

class PoolProcess;
class TCPServer;
class Buffer;

class Channel: public std::enable_shared_from_this<Channel>
{
    using CallBackFunction=std::function<void()>;

public:
    Channel(int fd,std::shared_ptr<Buffer>buf=nullptr)
        :fd_(fd),
        events_(EPOLLIN|EPOLLPRI),
        buf_(buf),
        timeCost_("channel: ")
    {
    }

    ~Channel();

    void  readAll(std::vector<char>&);

    void send(std::string& message);

    void dealEvent(epoll_event& event);  //事件提取

    const int events()const{return events_;}

    void enableWrite();

    

    

    Buffer* buf() {return buf_.get();};

    static void setPoolPro(std::shared_ptr<PoolProcess>pro);
    static void setServer(TCPServer*  server);

    int fd()const {return fd_;}

private:
    
    void disableWrite();
    
    void close();

    int fd_;
    int events_;

    Time::TimeRecord timeCost_;// 记录占用时间

    std::shared_ptr<Buffer>buf_;

private:

    static thread_local std::shared_ptr<PoolProcess> poolPro_;
    static TCPServer* server_;
};




#endif