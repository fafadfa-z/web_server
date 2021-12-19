#include "task.h"

TCPServer* Task::server_=nullptr;

Task::Task(std::function<void(TCPConnectionPtr)>task,ChannelPtr channel)
    :channel_(channel),task_(task)
{

}

