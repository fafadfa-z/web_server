#include "logger.h"
#include "timePoint.h"
#include "LocalMessage.h"
#include "assert.h"
#include <iostream>

namespace Log
{
    LogThread *entity = nullptr;

    const int logMessageSize = 384; //每条日志的最大长度

    char logMessageBuffer[logMessageSize];

    LogThread* init()
    {
        return LogThread::init();
    }


    Logger::Logger(std::function<void(char *, int)> cb)
        : fileN(maxNameSize, '0'), funN(maxNameSize, '0'), levelS(maxNameSize, '0')
    {
        stream_.setCallBack(cb);
    }

    const char *Logger::levelToStr(const LogLevel level)
    {
        switch (level)
        {
        case INFO_:
            return "Info: ";
        case DEBUG_:
            return "Debug: ";
        case FATAL_:
            return "Fatal: ";
        case HTTP_:
            return "HTTP: ";
        }
        return nullptr;
    }

    unsigned int Logger::findFileName(const char *FileName)
    {
        const char *slash = strrchr(FileName, '/');

        assert(slash);

        return static_cast<int>(slash - FileName) + 1;
    }

    // std::string Logger::dealFunName(const char * funName)
    // {
    //     return ans;
    // }

    void Logger::dealLogStr(const char *str, std::string &buf, int maxSize) //将字符串格式化成固定长度
    {

        if (maxSize > maxNameSize - 1)
            maxSize = maxNameSize - 1;

        auto size = strlen(str);

        if (size > maxSize)
            size = maxSize;

        ::memcpy(&buf[0], str, size);

        
        if(maxSize>size)
        {
            ::memcpy(&buf[size],"                                                               ",maxSize-size);
        }
 
        buf[maxSize]='\0';
    }


    LogStream &Logger::receive(const char *fileName, int line, const char *funName, LogLevel level)
    {
        Time::timePoint time(Time::getNowTime());

        dealLogStr(fileName + findFileName(fileName),fileN);

        dealLogStr(funName,funN);

        dealLogStr(levelToStr(level),levelS,10);

        // ::snprintf(logMessageBuffer, logMessageSize, "%s\t%-5d\t%-15.15s\t%-5d\t%-15.15s\t%-10.10s",
        //            time.toLogString().c_str(), getTid(), fileName + findFileName(fileName), line, funName, levelToStr(level));

        //stream_<<logMessageBuffer;

        stream_<< time.toLogString()<<"\t"<<getTid()<<"\t"<<&fileN[0]<<"\t"<<line<<"\t"<<&funN[0]<<"\t"<<&levelS[0];

        return stream_;
    }

}
