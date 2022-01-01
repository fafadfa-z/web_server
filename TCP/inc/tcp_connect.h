#ifndef TCP_CONNECT_H
#define TCP_CONNECT_H

#include <memory>
#include <functional>
#include <string>
#include "buffer.h"
#include "channel.h"

class TCPConnection
{
public:
 
    TCPConnection(Channel*);
    ~TCPConnection();

    auto read(char* index)
    {
        buffer_->clear(index);
    }

    void send(std::string& message) {channel_->send(message);}

    auto buffer()const 
    { 
        char* const index1=buffer_->readBuf_+buffer_->readIndex1_;
        char* const index2=buffer_->readBuf_+buffer_->readIndex2_;

        return std::pair(index1,index2);
    };

    void send(const char* message)
    {
            std::string temp(message);
            
            send(temp);
    }

    void clearBuf() {buffer_->clear();} //舍弃所有数据
     
    auto fd()const {return channel_->fd();}

private:
    Channel* channel_;
    Buffer*  buffer_;
};

#endif