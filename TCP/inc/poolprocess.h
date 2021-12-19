#ifndef POOL_PROCESS_H
#define POOL_PROCESS_H




#include <map>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>

#include <assert.h>
#include "logger.h"
#include "task.h"
#include "safe_queue.h"



class PoolProcess: public std::enable_shared_from_this<PoolProcess>
{
public:

    // using MapType=std::map<int,std::shared_ptr<Channel>>;


    PoolProcess();
  
    ~PoolProcess();

    void operator()();

    void pushConnect(Task& task)
    {
        LOG_INFO << "push task" << log::end;
        que_.push(std::move(task));
    }


private:
    void workLoop();

    SafeQueue<Task>que_;

};




#endif