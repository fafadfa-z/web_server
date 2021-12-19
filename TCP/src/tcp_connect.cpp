#include "tcp_connect.h"
#include "server.h"
#include "channel.h"
#include "logger.h"

TCPConnection::TCPConnection(std::shared_ptr<Channel> channel)
    :channel_(channel)
{
}




TCPConnection::~TCPConnection()
{
    LOG_DEBUG << "tcp connection 析构..."<<log::end;
}