#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "tcpAddr.h"
#include <functional>
#include <atomic>
#include <thread>
#include <memory>
#include <map>

class Channel;

using TaskFun=std::function<void(std::shared_ptr<Channel>)>;


class acceptor: public std::enable_shared_from_this<acceptor>
{
public:

    using MapType=std::map<int,std::shared_ptr<Channel>>;
 
    acceptor(TCPAddr,TaskFun,TaskFun);

    virtual  ~acceptor();

    bool begin(int fd);

    const auto fd()const {return listenFd_;}

    bool begin()
    {
        return begin(createListenSocket());
    }

    const TCPAddr& addr()const {return addr_;}

    void closeLoop() { looping_=false;};    //直接将标志位置零
    friend class Channel;

public:

    virtual void enable_write(int)=0;  //开始写入

    virtual void diseable_write(int)=0; // 结束写入

    virtual void removeFd(int)=0;   //删除一个文件描述符

protected:
    virtual bool acceptInit()=0;  //初始化操作

    virtual void acceptLoop()=0;  //接收操作

    virtual int createListenSocket();  //创建监听socket

    int listenFd_;

    TaskFun newConnCallBack_;
    TaskFun readableCallBack_;
    

    MapType channelMap_;

    int nums_;

private:

    void Loop();

    std::atomic<bool> looping_;

    TCPAddr addr_;

    std::thread acceptThread_;



};

#endif