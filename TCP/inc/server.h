#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <memory>
#include <thread>
#include <iostream>
#include <map>
#include <string>
#include <arpa/inet.h>


#include "thread_pool.h"
#include "tcp_connect.h"
#include "epoll_conn.h"
#include "acceptor.h"


class  Channel;
class TCPAddr;


class TCPServer
{
public:
    using TcpAddrPtr        = std::shared_ptr<TCPAddr>;
    using ChannelPtr  = std::shared_ptr<Channel>;
    using BufferPtr         = std::shared_ptr<Buffer>;


    using NewconnFunction   = std::function<void(TCPConnectionPtr)>;
    using ReadableFunction  = std::function<void(TCPConnectionPtr)>;

    static std::shared_ptr<TCPServer> init(TcpAddrPtr listenAddr, int num);

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
   
    void connNew(std::shared_ptr<Channel>);

    void connReadable(std::shared_ptr<Channel>);

    // void removeConnection(std::string index);

    TCPServer(TcpAddrPtr listenAddr, int threadNum);

private:
    std::shared_ptr<ThreadPool> pool_;      //线程池

    std::shared_ptr<acceptor> connRecv_;    //io副用

    TcpAddrPtr listenAddr_;                 //监听的地址

    ReadableFunction readableCallBack_;
    NewconnFunction  connectCallBack_;
    std::function<void()> CloseCallBack_;
    
    const int threadNum_;   //当前线程池内线程个数
    
    static std::shared_ptr<TCPServer> entity_;
};



#endif