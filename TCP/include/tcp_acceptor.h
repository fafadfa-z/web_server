#ifndef _TCP_ACCEPTOR_H
#define _TCP_ACCEPTOR_H

#include "tcp_addr.h"
#include <functional>
#include <atomic>
#include <thread>

class Acceptor
{
public:
    Acceptor(std::function<void(int)>);

    virtual  ~Acceptor();

    bool begin(int fd);

    const auto fd()const {return listenFd_;}

    bool begin()
    {
        return begin(createListenSocket());
    }

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

    std::thread acceptThread_;
};

#endif