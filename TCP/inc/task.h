#ifndef TASK_H
#define TASK_H

#include <memory>
#include <functional>
#include <assert.h>
#include "logger.h"
#include "tcp_connect.h"

class TCPServer;
class Channel;


using ChannelPtr = std::shared_ptr<Channel>;

class Task
{
public:
    Task(std::function<void(TCPConnectionPtr)>, ChannelPtr channelPtr);

    Task(const Task &) = default;
    Task& operator=(const Task & another )=default;


    void operator()()
    {
        assert(task_);

        TCPConnectionPtr conn(std::make_shared<TCPConnection>(channel_));
        
        task_(std::move(conn));
    }

    

    void setServer(TCPServer *server) { server_ = server; }

private:
    std::function<void(TCPConnectionPtr)> task_;
    ChannelPtr channel_;

    static TCPServer *server_;
};

#endif