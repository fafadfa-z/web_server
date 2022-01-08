#include "LogBuffer.h"
#include  "string.h"


namespace Log
{
    thread_local char LogBuffer::data_[bufferSize]={0};   //静态成员的构造顺序在这里会不会有影响。
    thread_local int LogBuffer::writeIndex_=0;


}

