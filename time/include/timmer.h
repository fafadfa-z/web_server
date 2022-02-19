#ifndef _TIMMRE_H
#define _TIMMER_H

#include<functional>
#include<set>
#include<vector>
#include<mutex>

//定时器线程
namespace Time
{
 
   class Timmer
    {
    public:

        using timeDuration=std::chrono::duration<int,std::milli>;

        Timmer(int);

        Timmer(const Timmer& another)=delete;

        Timmer& operator=(const Timmer&another)=delete;

        ~Timmer();

        void start();

        void addPeriodTask(std::function<void()>cb){PeriodTaskVec_.push_back(cb);};

        void addOnceTask(std::function<void()>cb)
        {
            std::lock_guard<std::mutex>guard(mut_);
            onceTaskVec_.push_back(cb);
        };

        int timeInterval(){return timeInterval_;}
   
        timeDuration useTime(){return useTime_;}

    private:

        const int timeInterval_;  //定时器步长(单位 毫秒)

        std::vector<std::function<void()>>PeriodTaskVec_; 

        std::vector<std::function<void()>>onceTaskVec_;

        bool working_;

        std::mutex mut_;  //用来保护 onceTaskVec_

       timeDuration useTime_; 
    };
}

#endif