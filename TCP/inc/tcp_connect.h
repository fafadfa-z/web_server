#ifndef TCP_CONNECT_H
#define TCP_CONNECT_H

#include <memory>
#include <functional>
#include <filesystem>
#include <string>
#include "buffer.h"
#include "channel.h"

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

    void sendBigMes(std::filesystem::path);   //直接发送一个文件

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