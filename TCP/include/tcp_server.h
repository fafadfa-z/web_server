#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include <memory>
#include <thread>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <cassert>

#include <arpa/inet.h>
#include <assert.h>

#include "tcp_thread_pool.h"
#include "tcp_connect.h"
#include "tcp_acceptor_epoll.h"
#include "tcp_acceptor.h"


class  Channel;
class TCPAddr;

using TCPConnectionPtr=TCPConnection*;

class TCPServer
{
public:
    using TcpAddrPtr        = std::shared_ptr<TCPAddr>;
    using ChannelPtr  = std::shared_ptr<Channel>;
    using BufferPtr         = std::shared_ptr<Buffer>;
    using NewconnFunction   = std::function<void(TCPConnection*)>;
    using ReadableFunction  = std::function<void(TCPConnection*)>;

    static std::shared_ptr<TCPServer> init(int num);

    ~TCPServer();

    friend Channel;

    void beginServer();

    void setReadableCallBack(ReadableFunction task) 
    { 
        readableCallBack_ = task; 
    }
    void setConnectCallBack(NewconnFunction task) 
    { 
        connectCallBack_ = task; 
    }

private:
   
    void connNew(Channel*);

    void connReadable(Channel*);

    // void removeConnection(std::string index);

    TCPServer(int threadNum);

private:
    std::shared_ptr<ThreadPool> pool_;      //线程池

    std::unique_ptr<Acceptor> connRecv_;    //io副用

    ReadableFunction readableCallBack_;
    NewconnFunction  connectCallBack_;
    std::function<void()> CloseCallBack_;
    
    const int threadNum_;   //当前线程池内线程个数
    
    static std::shared_ptr<TCPServer> entity_;
};



#endif