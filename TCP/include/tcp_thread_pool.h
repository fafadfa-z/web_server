#ifndef _TCP_THREAD_POOL_H
#define _TCP_THREAD_POOL_H

#include <vector>
#include <thread>
#include <functional>
#include <memory>
#include <mutex>
#include <assert.h>

#include "tcp_thread_pool_process.h"

class TCPConnection;


#if 0  //抽象基类，先不用
class PoolFunction
{
public:
    virtual void operator()()=0;

    virtual void addConnection(const TCPConnection&);

    virtual void close();
};
#endif


class ThreadPool
{
    
public:

    static std::shared_ptr<ThreadPool> init(int);
    void pushConnect(const int);

    void begin();

    

    ~ThreadPool();

private:

    ThreadPool(int threadNum)
        :current_(0),
        threadNum_(threadNum) 
    {
        
    }

    std::vector<std::shared_ptr<PoolProcess>> poolProcesses_;

    std::vector<std::thread>threadList;

    int current_;
private:
    inline static std::shared_ptr<ThreadPool> entity_=nullptr;
    const int threadNum_;
};

#endif