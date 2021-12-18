#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include<queue>

using std::queue;

template<typename T>
class MyQueue
{
public:
    virtual bool empty();
    
    virtual bool try_pop(T&);

    virtual bool push(T);

    virtual ~MyQueue()=0;    // effective 中写过将析构函数定义为纯虚函数?
private:
    queue<T>que_;   


};

#endif