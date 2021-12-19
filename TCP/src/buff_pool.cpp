#include "buff_pool.h"
// #include <linux/gfp.h>


static const int buf2kNum = 10'000; // 2k的内存页一共 10k 个，共20m
static const int buf4kNum = 1'000;  // 2k的内存页一共 1k 个，共4m'
static const int buf8kNum = 500;
static const int buf16kNum = 200;

BuffPoolPtr BufferPool::entity_ = nullptr;

BuffPoolPtr BufferPool::init(std::vector<int> &vec)
{
    if (entity_ == nullptr)
    {
         entity_.reset(new BufferPool(vec));
    }
    return entity_;
}

BufferPool::BufferPool(std::vector<int>&threadId)
    :threadId_(threadId)
{
    




}

BufferPool::~BufferPool()
{



}