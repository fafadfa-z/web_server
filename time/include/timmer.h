#ifndef _TIMMRE_H
#define _TIMMER_H

#include <functional>
#include <vector>
#include <list>
#include <atomic>
#include <mutex>

//定时器线程
namespace Time
{
    class TimerTask //定时器任务
    {
    public:
        TimerTask(int time,bool repeatable = false,int id = 0);

        virtual ~TimerTask(){}

        virtual void work() = 0;


        long timePoint()const {return timePoint_;}

    protected:

        long timePoint_;  //记录应该执行的时间
        long insertTime_; //记录放入定时器中的时间

        bool repeatable_; //是否是重复事件

        int id_; //便于以后用来删除
    };

    //需要传入时间的版本
    class NeedTime : public TimerTask
    {
    public:
        NeedTime(int time,int id = 0)
            :TimerTask(time,id)
        {}

        void setTask(std::function<void(int)>task){task_ = task;}
        
        void work() override;


        std::function<void(int)>task_;
    };

    //不需要时间的版本
    class NotNeedTime : public TimerTask
    {
    public:
        NotNeedTime(int time,int id = 0)
            :TimerTask(time,id)
        {}

        void setTask(std::function<void()>task){task_ = task;}

        void work() override
        {
            task_();
        }

        std::function<void()>task_;
    };


    // 定时器类，分辨率为10ms
    class Timmer
    {
    public:
        using timeDuration = std::chrono::duration<int, std::milli>;

        Timmer(int);

        Timmer(const Timmer &another) = delete;

        Timmer &operator=(const Timmer &another) = delete;

        ~Timmer();

        void start();

        void addOnceTask(std::function<void()>, int);
        void addOnceTask(std::function<void(int)>, int);



    private:
        const int timeNum_;

        std::vector<std::list<TimerTask*>> timeWheel_; //时间轮的数据结构

        std::vector<std::mutex> mutexVec; //为时间轮每一个扇叶申请一个互斥锁

        int wheelIndex_; //时间轮指针

        bool working_;
    };
}

#endif