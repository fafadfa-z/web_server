#include "LogStream.h"
#include "assert.h"
#include <algorithm>
#include <iostream>

namespace log
{
    
    const int LogStream::maxNumSize=48;
   // const short LogStream::maxLineSize=128;

    const char digits[] = "9876543210123456789";
    const char *zero = digits + 9;
    static_assert(sizeof(digits) == 20, "wrong number of digits");

    template <typename T>
    size_t LogStream::convert(char buf[], T value)
    {
        T i = value;
        char *p = buf;
        do
        {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);

        if (value < 0)
        {
            *p++ = '-';
        }
        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }

    LogStream &LogStream::operator<<(short value)
    {
        *this << static_cast<int>(value);

        return *this;
    }
    LogStream &LogStream::operator<<(int value)
    {
        char buf[maxNumSize] = {0};

        int size = convert(buf, value);
        assert(size <= maxNumSize);

        buf_.writeIn(buf, size);
        return *this;
    }
    LogStream &LogStream::operator<<(long value)
    {
        char buf[maxNumSize] = {0};

        int size = convert(buf, value);
        assert(size <= maxNumSize);

        buf_.writeIn(buf, size);
        return *this;
    }

    LogStream &LogStream::operator<<(long long value)
    {
        char buf[maxNumSize] = {0};

        int size = convert(buf, value);
        assert(size <= maxNumSize);

        buf_.writeIn(buf, size);
        return *this;
    }

    LogStream &LogStream::operator<<(unsigned short value)
    {
        *this << static_cast<unsigned int>(value);
        return *this;
    }
    LogStream &LogStream::operator<<(unsigned int value)
    {
        char buf[maxNumSize] = {0};

        int size = convert(buf, value);
        assert(size <= maxNumSize);

        buf_.writeIn(buf, size);
        return *this;
    }
    LogStream &LogStream::operator<<(unsigned long value)
    {
        char buf[maxNumSize] = {0};

        int size = convert(buf, value);
        assert(size <= maxNumSize);

        buf_.writeIn(buf, size);
        return *this;
    }
    LogStream &LogStream::operator<<(unsigned long long value)
    {
        char buf[maxNumSize] = {0};

        int size = convert(buf, value);
        assert(size <= maxNumSize);

        buf_.writeIn(buf, size);
        return *this;
    }

    LogStream &LogStream::operator<<(double value)
    {
        char buf[maxNumSize] = {0};
        int size = snprintf(buf, maxNumSize, "%.12g", value);

        buf_.writeIn(buf, size);

        return *this;
    }



    LogStream &LogStream::operator<<(float value)
    {
         *this << static_cast<double>(value);
        return *this;
    }

    LogStream& end(LogStream& stream)
    {
        char *buf=stream.buf_.data(); 
        int size=stream.buf_.writeIndex();
        buf[size]='\r';
        buf[size+1]='\n';

        assert(stream.updateCallBack_);

        stream.updateCallBack_(buf,size+2);

        stream.buf_.refresh();
        
        return stream;
    }


}