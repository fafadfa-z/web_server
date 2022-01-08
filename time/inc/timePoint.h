#ifndef TIME_POINT_H
#define TIME_POINT_H

#include <string>
#include <chrono>
#include "logger.h"


namespace Time
{
    const int usTos = 1000'000; //微妙到秒的进制转换

    using namespace std::chrono;

    class timePoint
    {
    public:
        using clockType = system_clock; // 先用着 system_clock

        timePoint(time_point<system_clock> time)
            : time_(time)
        {
        }
        std::string toString();

        std::string toLogString();

        std::string toLogFileName();

    private:
        time_point<system_clock> time_;
    };

    inline time_point<system_clock> getNowTime()
    {
        return system_clock::now();
    }


    //输出调试信息。
    class TimeRecord
    {
    public:
        TimeRecord(const char* logMes="No name time recodes")
            :logMes_(logMes)
        {
            beginTime_=system_clock::now();
        }
        
        //TimeRecord(const std::string& );

        ~TimeRecord()
        {
            auto endTime=system_clock::now();

            auto timeCost=duration_cast<microseconds>(endTime - beginTime_).count();

            LOG_DEBUG<<logMes_<<" cost time: "<<timeCost<<" us"<<Log::end;
        }

    private:

        time_point<system_clock> beginTime_;
        const char* logMes_;
    };

} // namespace Time

#endif