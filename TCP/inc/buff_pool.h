#ifndef BUFF_POOL_H
#define BUFF_POOL_H


#include<memory>
#include<vector>
#include <stdlib.h>
#include <assert.h>
#include <stack>

class BufferPool;

using BuffPoolPtr=std::shared_ptr<BufferPool>;


struct ThreadBufMes //储存单线程当前内存池的大小�?
{
    /*---已使用的大小---*/
    int used2k_=0;
    int used4k_=0;
    int used8k_=0;
    int used16k_=0;

    char*  buf2k_=nullptr;
    char*  buf4k_=nullptr;
    char*  buf8k_=nullptr;
    char*  buf16k_=nullptr;

    std::stack<char*>st2k_;
    std::stack<char*>st4k_;
    std::stack<char*>st8k_;
    std::stack<char*>st16k_;
    
    int threadId_=0;


};


class BufferPool
{

public:

    BufferPool();

    BufferPool& operator=(const BufferPool&)=delete;

    BufferPool(const BufferPool&)=delete;

    ~BufferPool();

    int getBuf(char*&,int);
    void freeBuf(char*, int);

    int changeBuf(char*&buf,int oldSize,int newSize)
    {
        char* temp;
        auto realSize=getBuf(temp,newSize);

        std::copy(buf,buf+oldSize,temp);

        freeBuf(buf,oldSize);
        return realSize;
    }
    private:
    ThreadBufMes localBuf_;
};



#endif