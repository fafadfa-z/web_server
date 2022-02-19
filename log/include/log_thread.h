#ifndef _LOG_THREAD_H
#define _LOG_THREAD_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include <memory>

#include <string.h>
#include <utility>
#include <unistd.h>
#include <cassert>

#include "log_file.h"

namespace Log
{

    class Logger;

    class LogThread
    {
    public:
        ~LogThread();

        static LogThread *init();

        void apendMessage(char *line, unsigned int size); //这里size不包含 '\0'

        unsigned int freeSize() { return static_cast<unsigned int>(maxBufSize - currentBufIndex_); }

        void changeBuf();

        void logLoop();

        std::shared_ptr<Logger> logger;

    private:
        LogThread();
        void flushToFile(std::vector<std::pair<char *, int>> &buf);
        void deleteBuf(std::vector<std::pair<char *, int>> &buf, size_t beginIndex);

        LogFile logFile_;

        bool logging_;

        char *currentBuf_;
        int currentBufIndex_;
        char *preBuf_;

        std::vector<std::pair<char *, int>> readyBuf_;
        std::vector<std::pair<char *, int>> bufToWrite_;

        std::mutex mut_;

        static const int maxBufSize;
    };


    extern const char *logPath;
}

#endif
