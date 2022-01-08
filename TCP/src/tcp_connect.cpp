#include "tcp_connect.h"
#include "server.h"
#include "channel.h"
#include "logger.h"

TCPConnection::TCPConnection(Channel* channel)
    :channel_(channel),buffer_(channel_->buf())
{


}




TCPConnection::~TCPConnection()
{
    LOG_DEBUG << "tcp connection 析构..."<<Log::end;
}