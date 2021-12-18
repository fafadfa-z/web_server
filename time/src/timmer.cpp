#include "timmer.h"
#include <unistd.h>
#include <thread>
#include <chrono>

#include "timePoint.h"

namespace Time
{

    Timmer::Timmer(int timeInterval)
        : timeInterval_(timeInterval),working_(false)
    {
    }

    Timmer::~Timmer()
    {
    }

    void Timmer::start()
    {
        if(working_) return;

        working_=true;

       std::vector<std::function<void()>> temp;

        while(working_)
        {   
            std::this_thread::sleep_for(std::chrono::milliseconds(timeInterval_));

            auto begin=getNowTime();

            for(auto task:PeriodTaskVec_) task();
            
            {
                std::lock_guard<std::mutex>guard(mut_);

                if(!onceTaskVec_.empty())
                {
                    temp.swap(onceTaskVec_);
                }
            }

            for(auto task:temp) task();
            
            temp.clear();

            auto end=getNowTime();

            useTime_=std::chrono::duration_cast<timeDuration>(end-begin);   // 记录消耗的时间
        }
    }
}
