#ifndef TCP_CONNECT_H
#define TCP_CONNECT_H

#include <memory>
#include <functional>
#include <string>
#include "buffer.h"
#include "channel.h"




// 只是对外提供的接口
class TCPConnection
{
public:
 
    TCPConnection(Channel*);
    ~TCPConnection();

    auto readBuf(std::vector<char>&vec)
    {
          return channel_->readBuf(vec);
    }

    void send(std::string& message)
    {
        channel_->send(message);
    }

    void send(const char* message)
    {
        std::string temp(message);
        
        send(temp);
    }
    
    auto fd()const {return channel_->fd();}

private:
    Channel* channel_;
};

#endif