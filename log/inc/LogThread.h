#ifndef LOG_THREAD_H
#define LOG_THREAD_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include <memory>

#include "LogFile.h"

namespace Log
{

    class Logger;

    class LogThread
    {
    public:
        ~LogThread();

        static LogThread *init();

        void apendMessage(char *line, int size); //这里size不包含 '\0'

        unsigned int freeSize() { return static_cast<int>(maxBufSize - currentBufIndex_); }

        void changeBuf();

        void logLoop();

        std::shared_ptr<Logger> logger;

    private:
        LogThread();
        void flushToFile(std::vector<std::pair<char *, int>> &buf);
        void deleteBuf(std::vector<std::pair<char *, int>> &buf, int beginIndex);

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
