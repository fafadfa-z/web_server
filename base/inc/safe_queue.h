#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <queue>
#include <mutex>



template<typename Type_>
class SafeQueue
{
public:
    bool empty()
    {
        std::lock_guard<std::mutex>guard(mut_);
        return queue_.empty();
    }

    // void push(Type_ task)
    // {
    //     std::lock_guard<std::mutex>guard(mut_);

    //     queue_.push(std::move(task));
    // }
    void push(Type_&& task)
    {
        std::lock_guard<std::mutex>guard(mut_);

        queue_.push(task);
    }

    bool try_pop(Type_& task)
    {
        std::lock_guard<std::mutex>guard(mut_);

        if(queue_.empty()) return false;

        task=std::move(queue_.front());
        
        queue_.pop();
    }



    void swap(std::queue<Type_>&que)
    {
        std::lock_guard<std::mutex>guard(mut_);

        queue_.swap(que);
    }

private:

    std::queue<Type_>queue_;

    std::mutex mut_;
    
};



#endif