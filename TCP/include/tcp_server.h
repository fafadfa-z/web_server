#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include <memory>
#include <thread>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <atomic>

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

    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

    static std::shared_ptr<TCPServer> init(int num,bool disMes = false);

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

    void connNewFd(int,TimePoint); // 记录新的fd

    void connDelFd(int);  //断开连接时记录信息

    void disMes(long); //定时函数, 处理统计信息




    void connReadable(Channel*);

    TCPServer(int threadNum,bool disMes);

private:
    std::shared_ptr<ThreadPool> pool_;      //线程池

    std::unique_ptr<Acceptor> connRecv_;    //io副用

    ReadableFunction readableCallBack_;
    NewconnFunction  connectCallBack_;
    std::function<void()> CloseCallBack_;
    
    const int threadNum_;   //当前线程池内线程个数

private: //记录统计信息

    std::mutex mut_;
    std::map<int,TimePoint>fdMap_; // 记录每个fd连接的时间

    std::atomic<bool> recordFlag_;   //开始统计的标志位
    long recordNum_;    //单次统计时间内的连接数
    long maxNum_;       //单次统计内的最多连接数 
    long currentNum_;   //当前的连接数

    long totalTime_; // 记录总处理时间

    static std::shared_ptr<TCPServer> entity_;

    const bool disMes_;
};



#endif