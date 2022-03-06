#ifndef _LOG_BUFFER_H
#define _LOG_BUFFER_H

#include "string.h"
#include <map>

namespace Log
{

    constexpr int bufferSize = 10*1024;
    constexpr int headLevel=bufferSize-3;
    constexpr int lowLevel=bufferSize-256;


    class LogBuffer 
    {
    public:
        LogBuffer()
        {
        }

        bool writeIn(const char *line, unsigned int size);


        bool writeEnd();

        std::pair<char*,int>updateMes(); //返回当前缓冲索引

        void refresh(){writeIndex_=0;}

    private:
        inline static thread_local char data_[bufferSize]={0};
        inline static thread_local int writeIndex_;
    };

} //namespace Log

#endif