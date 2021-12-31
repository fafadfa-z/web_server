#include "thread_pool.h"
#include "logger.h"
#include "assert.h"
#include "buffer.h"



std::shared_ptr<ThreadPool> ThreadPool::entity_=nullptr;

std::shared_ptr<ThreadPool> ThreadPool::init(int threadNum)
{
    if(entity_==nullptr)
    {
        entity_.reset(new ThreadPool(threadNum));
    }

    return  entity_; 
}

void ThreadPool::begin()
{
    LOG_DEBUG<<"Thread pool begin!  threadNum:"<<threadNum_<<log::end;

    std::vector<std::thread::id>idVec;
 
    for(int i=0;i<threadNum_;i++)
    {
        poolProcesses_.push_back(std::make_shared<PoolProcess>());

        threadList.push_back(std::thread([this,i]{(poolProcesses_[i])->operator()();}));

        threadList[i].detach();

        idVec.push_back(threadList[i].get_id());
    }

}

void ThreadPool::pushConnect(const int fd)
{
    if(current_==threadNum_) current_=0; 

    (poolProcesses_[current_])->pushConnect(fd);
    
    current_++;
}


ThreadPool::~ThreadPool()
{
    LOG_DEBUG<<"ThreadPool 析构"<<log::end;

}