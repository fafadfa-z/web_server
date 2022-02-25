#ifndef _SAFE_QUEUE_H
#define _SAFE_QUEUE_H

#include<deque>
#include<mutex>
#include<optional>
using std::deque;



namespace Base
{
    template<typename T>
    class MyQueue
    {
    public:
        MyQueue(int size)
            :maxSize_(size)
        {

        }


        bool empty()
        {
            std::lock_guard<std::mutex>guard(mut_);
            return que_.empty();
        }
    
        std::optional<T> try_pop()
        {
            std::lock_guard<std::mutex>guard(mut_);
            if(que_.empty())
                return std::nullopt;

            auto ret=std::move(que_.back()); 
            que_.pop_back();
            return ret;
        }

        bool push(T&& element)
        {
            std::lock_guard<std::mutex>guard(mut_);
            if(que_.size()==maxSize_)
                return false;

            que_.push_back(std::move(element));
            return true;
        }
        int size()
        {
            std::lock_guard<std::mutex>guard(mut_);
            return que_.size();
        }

    private:
        deque<T>que_;   
        size_t maxSize_;
        std::mutex mut_;
    };
}

#endif