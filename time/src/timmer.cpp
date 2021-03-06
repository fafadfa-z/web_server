#include "timmer.h"
#include <thread>
#include <chrono>

#include <iostream>

namespace Time
{
    using namespace std::chrono;

    void delay_ms(int ms)
    {
        auto begin = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        begin += ms;

        while (begin > duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())
        {
            std::this_thread::yield();
        }
    }

    TimerTask::TimerTask(int time, bool repeatable, int id)
        : repeatable_(repeatable),
          id_(id)
    {
        auto nowTime = system_clock::now().time_since_epoch();

        insertTime_ = duration_cast<milliseconds>(nowTime).count();

        timePoint_ = insertTime_ + time;
    }

    void NeedTime::work()
    {
        auto now = system_clock::now().time_since_epoch();
        auto timeDur = duration_cast<milliseconds>(now).count();

        task_(timeDur - insertTime_);
    }

    Timmer *Timmer::init(int time)
    {
        if (entity_ == nullptr)
            entity_ = new Timmer(time);

        return entity_;
    }

    Timmer::Timmer(int timeInterval)
        : timeNum_(1000 / timeInterval),
          timeWheel_(timeNum_),
          mutexVec(timeNum_),
          wheelIndex_(0),
          working_(false)
    {
    }

    Timmer::~Timmer()
    {
        for (auto &list : timeWheel_)
        {
            for (auto &timer : list)
            {
                delete timer;
            }
        }
    }

    void Timmer::addOnceTask(std::function<void()> task, int time) // time 单位：ms
    {
        int index = time;

        index /= 1000 / timeNum_; //计算几个槽位之后进行

        index %= timeNum_; //去掉整圈的克度，需要向下移动多少个槽位

        index = (index + wheelIndex_) / timeNum_; //计算需要放入的槽位

        auto *timer = new NotNeedTime(time);

        timer->setTask(task);

        std::lock_guard guard(mutexVec[index]);

        timeWheel_[index].push_back(timer);
    }

    void Timmer::addOnceTask(std::function<void(int)> task, int time)
    {
        int index = time;

        index /= 1000 / timeNum_;

        index %= timeNum_;

        index = (index + wheelIndex_) / timeNum_;

        auto *timer = new NeedTime(time);

        timer->setTask(task);

        std::lock_guard guard(mutexVec[index]);

        timeWheel_[index].push_back(timer);
    }

    bool Timmer::addPriodTask(std::function<void()> task, int time, int id)
    {
        NotNeedTime *timer;
        {
            std::lock_guard guard(priodTimerGuarder_);

            auto iter = priodTimerMap_.find(id);

            if (iter != priodTimerMap_.end())
                return false;

            timer = new NotNeedTime(time, true, id);

            priodTimerMap_.insert(std::move(std::pair{id, timer}));
        }

        int index = time;

        index /= 1000 / timeNum_;

        index %= timeNum_;

        index = (index + wheelIndex_) / timeNum_;

        timer->setTask(task);

        std::lock_guard guard(mutexVec[index]);

        timeWheel_[index].push_back(timer);

        return true;
    }

    bool Timmer::addPriodTask(std::function<void(int)> task, int time, int id)
    {
        NeedTime *timer;

        {
            std::lock_guard guard(priodTimerGuarder_);

            auto iter = priodTimerMap_.find(id);

            if (iter != priodTimerMap_.end())
                return false;

            timer = new NeedTime(time, true, id);

            priodTimerMap_.insert(std::move(std::pair{id, timer}));
        }

        int index = time;

        index /= 1000 / timeNum_;

        index %= timeNum_;

        index = (index + wheelIndex_) / timeNum_;

        timer->setTask(task);

        std::lock_guard guard(mutexVec[index]);

        timeWheel_[index].push_back(timer);

        return true;
    }

    void Timmer::start()
    {
        if (working_)
            return;

        working_ = true;

        long used = 0; // 矫正时钟，记录每次使用了多长时间。

        long timeBegin = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        while (working_)
        {
            delay_ms(1000 / timeNum_ - used);

            long pre = timeBegin;

            timeBegin = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

            used = timeBegin - pre - 10; //计算上次运行了多长时间，用来校准定时器

            used = std::max(used, 0l);

            std::vector<TimerTask *> timeOutVec;
            timeOutVec.reserve(5);

            mutexVec[wheelIndex_].lock();

            auto &wheelList = timeWheel_[wheelIndex_];

            for (auto iter = wheelList.begin(); iter != wheelList.end(); iter++)
            {
                auto timer = *iter;
                if (timer->timePoint() <= timeBegin)
                {
                    timeOutVec.push_back(timer);
                    iter = wheelList.erase(iter); //小心这里迭代器失效的问题
                }
            }
            mutexVec[wheelIndex_].unlock();

            for (auto timer : timeOutVec) //执行超时的定时器。
            {
                timer->work();

                delete timer;
            }

            timeOutVec.clear();

            wheelIndex_++;

            if (wheelIndex_ == timeNum_)
                wheelIndex_ = 0;
        }
    }
}
