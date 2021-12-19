
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <functional>
#include <memory>
#include <mutex>
#include "poolprocess.h"



class TCPConnection;
class Task;

#if 0  //抽象基类，先不使用
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
    void pushConnect(Task& task);

    void begin();

    

    ~ThreadPool();

private:

    ThreadPool(int threadNum)
        :threadNum_(threadNum),
        current_(0)
    {
        
    }

    std::vector<std::shared_ptr<PoolProcess>> poolProcesses_;

    std::vector<std::thread>threadList;

    int current_;
private:

    const int threadNum_;
    static std::shared_ptr<ThreadPool> entity_;
};

#endif