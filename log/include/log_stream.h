#ifndef _LOG_STREAM_H
#define _LOG_STREAM_H

#include "log_buffer.h"
#include <string>
#include <functional>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <string_view>

namespace Log
{
   

    class LogStream
    {
    public:
        LogStream &operator<<(const char *str)
        {
            if (str)
                buf_.writeIn(str, strlen(str));
            else
                buf_.writeIn("(null)", 6);

            return *this;
        }

        LogStream &operator<<(std::string_view str)
        {
            buf_.writeIn(str.data(), str.size());
            return *this;
        }
        LogStream &operator<<(std::string&& str)
        {
            buf_.writeIn(str.c_str(), str.size());
            return *this;
        }

        LogStream &operator<<(short);
        LogStream &operator<<(int);
        LogStream &operator<<(long);
        LogStream &operator<<(long long);
        LogStream &operator<<(unsigned short);
        LogStream &operator<<(unsigned int);
        LogStream &operator<<(unsigned long);
        LogStream &operator<<(unsigned long long);
        LogStream &operator<<(float);
        LogStream &operator<<(double);

        LogStream &operator<<(LogStream &(*fun)(LogStream &))
        {
            fun(*this);
            return *this;
        }

        template <typename T>
        size_t convert(char buf[], T value);

        friend LogStream& end(LogStream& stream);

        void setCallBack(const std::function<void(char*,int)>&cb){updateCallBack_=cb;}

    private:
        
        std::function<void(char*,int)> updateCallBack_;

        static const int maxNumSize; // 数字转化为字符串的最大长度。
        static const short maxLineSize; // 整行记录的最大长度。
        LogBuffer buf_;
    };


} // namespace Log

#endif