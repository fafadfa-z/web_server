#include "log_buffer.h"
#include "string.h"

namespace Log
{
    bool LogBuffer::writeIn(const char *line, unsigned int size)
    {
        if (writeIndex_+size > headLevel)
            return false;

        ::memcpy(data_ + writeIndex_, line, size);

        writeIndex_ += size;

        return true;
    }
    bool LogBuffer::writeEnd()
    {
        data_[writeIndex_++]='\r';
        data_[writeIndex_++]='\n';

        return writeIndex_>=lowLevel;
    }
    std::pair<char*,int>LogBuffer::updateMes()
    {
        return {data_,writeIndex_};
    }


}
