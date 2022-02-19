#ifndef _TCP_CONNECT_H
#define _TCP_CONNECT_H

#include <memory>
#include <functional>
#include <filesystem>
#include <string>

#include "tcp_buffer.h"
#include "tcp_channel.h"

class TCPConnection
{
public:
    TCPConnection(Channel *);
    ~TCPConnection();

    auto read(char *index)
    {
        buffer_->clear(index);
    }

    void send(const std::string &message) { channel_->send(message);}

    void sendWithFile(const std::string &message,const std::filesystem::path& path)   //发送一个文件
    { channel_->sendWithFile(message,path); }

    auto buffer() const
    {
        char *const index1 = buffer_->readBuf_ + buffer_->readIndex1_;
        char *const index2 = buffer_->readBuf_ + buffer_->readIndex2_;

        return std::pair(index1, index2);
    };

    void send(const char *message)
    {
        const std::string temp(message);

        send(temp);
    }

    void clearBuf() { buffer_->clear(); } //������������

    auto fd() const { return channel_->fd(); }

private:
    Channel *channel_;
    Buffer *buffer_;
};

#endif