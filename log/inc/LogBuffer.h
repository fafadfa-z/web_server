#ifndef LOG_BUFFER_H
#define LOG_BUFFER_H

#include "string.h"

namespace Log
{

    const int bufferSize = 256;

    class LogBuffer 
    {
    public:
        LogBuffer()
        {
        }

        bool writeIn(const char *line, unsigned int size)
        {
            if (avilableSize() < size)
                return false;

            ::memcpy(data_+writeIndex_, line, size);

            writeIndex_ += size;
            return true;
        }

        char* data(){return data_;}

        int writeIndex(){return writeIndex_;}

        unsigned int avilableSize() { return static_cast<unsigned int>(bufferSize - writeIndex_); } //返回当前可写的大小

        void refresh(){writeIndex_=0;}

    private:
        static thread_local char data_[bufferSize];
        static thread_local int writeIndex_;
    };

} //namespace Log

#endif