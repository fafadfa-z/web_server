#ifndef _LOG_THREAD_H
#define _LOG_THREAD_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <memory>

#include <string.h>
#include <utility>
#include <unistd.h>
#include <cassert>

#include "log_file.h"
#include"safe_queue.h"

namespace Log
{

    class Logger;

    class LogThread
    {
        using BufType=std::deque<std::pair<std::vector<char>,int>>;
    public:
        ~LogThread();

        static LogThread *init();

        void apendMessage(char *, unsigned int ); //这里size不包含 '\0'

        int freeSize() { return bufSize_ - currentBufIndex_; }

        void changeBuf();

        void logLoop();

        std::shared_ptr<Logger> logger_;

    private:
        LogThread();
        void flushToFile();

        LogFile logFile_;

    private:

        std::vector<char> currentBuf_;
        
        Base::MyQueue<std::vector<char>> preBuf_;
        int currentBufIndex_;
        
        BufType readyBuf_;
        BufType bufToWrite_;

        std::mutex mut_;
        std::condition_variable waiter_;


    private:
        inline static const int bufSize_=100 * 1024;       //一块日志缓冲区的大小
        inline static const int bufNum_=5;                 //可以保存的内存块数
    };


    extern const char *logPath;
}

#endif
