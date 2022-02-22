#include "tcp_connect.h"
#include "tcp_server.h"
#include "tcp_channel.h"
#include "logger.h"



TCPConnection::TCPConnection(Channel* channel)
    :channel_(channel),buffer_(channel_->buf())
{


}




TCPConnection::~TCPConnection()
{
    LOG_INFO << "tcp connection 析构..."<<Log::end;
}