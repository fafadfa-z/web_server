#ifndef BUFF_POOL_H
#define BUFF_POOL_H


#include<memory>
#include<vector>
#include<thread>
#include<list>


class BufferPool;

using BuffPoolPtr=std::shared_ptr<BufferPool>;


struct ThreadBufMes //储存单线程当前线程池的大小。
{
    /*---可用大小。----*/
    int size2k_;
    int size4k_;
    int size8k_;
    int size16k_;
    /*---已使用的大小---*/
    int used2k_;
    int used4k_;
    int used8k_;
    int used16k_;

    int totalSize_;   //总大小，单位: kb
};


class BufferPool
{

public:
    static BuffPoolPtr init(std::vector<int>&);

    BufferPool& operator=(const BufferPool&)=delete;

    BufferPool(const BufferPool&)=delete;

    ~BufferPool();

private:

    BufferPool(std::vector<int>&);
    
    const std::vector<int>threadId_;

    std::vector<ThreadBufMes> threadMes_; //每个注册线程的信息
    ThreadBufMes totalMse;                //总信息


    static BuffPoolPtr entity_;
};



#endif