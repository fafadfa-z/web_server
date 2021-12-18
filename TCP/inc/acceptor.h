#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "tcpAddr.h"
#include <functional>
#include <atomic>
#include <thread>


class acceptor
{
public:
    acceptor(TCPAddr,std::function<void(int)>);

    virtual  ~acceptor();

    bool begin(int fd);

    const auto fd()const {return listenFd_;}

    bool begin()
    {
        return begin(createListenSocket());
    }

    const TCPAddr& addr()const {return addr_;}


    void closeLoop() { looping_=false;};    //直接将标志位置零

protected:
    virtual bool acceptInit()=0;  //初始化操作

    virtual void acceptLoop()=0;  //接收操作

    virtual int createListenSocket();

    int listenFd_;
    
    std::function<void(int)>submitCallBack_;

private:

    void Loop();

    std::atomic<bool> looping_;

    TCPAddr addr_;

    std::thread acceptThread_;
};

#endif