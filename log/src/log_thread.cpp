#include "log_thread.h"

#include "logger.h"

namespace Log
{

    const int LogThread::maxBufSize = 1000 * 1024;

    LogThread::LogThread()
        :logger(new Logger(std::bind(&LogThread::apendMessage,this,std::placeholders::_1,std::placeholders::_2))),
         currentBuf_(new char[maxBufSize]),
         currentBufIndex_(0),
         preBuf_(new char[maxBufSize])
    {   
        auto logThread = std::thread(std::bind(&LogThread::logLoop,this));

        logThread.detach();              
    }

    LogThread::~LogThread()
    {
        delete[] currentBuf_;
        delete[] preBuf_;

        for (auto temp : readyBuf_)
        {
            delete[] temp.first;
        }
    }

    LogThread *LogThread::init()
    {
        if (!entity)
        {
            entity = new LogThread;
        }
        return entity;
    }

    void LogThread::apendMessage(char *line, unsigned int size)
    {
        std::lock_guard<std::mutex> guard(mut_);
        auto temp=static_cast<unsigned int>(freeSize());
        if (temp < size)
            changeBuf(); //更换新的缓存

        ::memcpy(currentBuf_ + currentBufIndex_, line, size);
        currentBufIndex_ += size;
    }

    void LogThread::changeBuf()
    {
        readyBuf_.push_back(std::pair<char *, int>(currentBuf_, currentBufIndex_));

        if (preBuf_)
        {
            currentBuf_ = preBuf_;
            preBuf_ = nullptr;
        }
        else
        {
            std::cout << "额外的日至缓存...." << std::endl;
            currentBuf_ = new char[maxBufSize];
        }
        currentBufIndex_ = 0;
    }

    void LogThread::logLoop()
    {
        bool logging_ = true;

    std::cout << "Log thread begin...." << std::endl;
        while (logging_)
        {
            {
                if (readyBuf_.empty())
                    sleep(1);
                std::lock_guard<std::mutex> guard(mut_);

                //std::cout <<currentBuf_<< std::endl;

                changeBuf();

                assert(bufToWrite_.empty());

                bufToWrite_.swap(readyBuf_);
            }

            if (bufToWrite_.size() > 2)
            {
                std::cout << "Log system: too many Log...\n";

                bufToWrite_.erase(bufToWrite_.begin() + 2, bufToWrite_.end());
            }
            flushToFile(bufToWrite_);

            if (!preBuf_)
            {
                preBuf_ = bufToWrite_[0].first;

                deleteBuf(bufToWrite_, 1);
            }
            else
                deleteBuf(bufToWrite_, 0);

            logFile_.flush();
            bufToWrite_.clear();
        }
    }

    void LogThread::flushToFile(std::vector<std::pair<char*,int>>&buf)
    {
        for (auto temp : buf)
        {
            logFile_.writeMessage(temp.first, temp.second);
        }
    }
    void LogThread::deleteBuf(std::vector<std::pair<char *, int>> &buf, size_t beginIndex)
    {
        size_t size=static_cast<size_t>(buf.size());
        for (size_t i = beginIndex; i < size; i++)
        {
            delete[] buf[i].first;
        }

        buf.clear();
    }

} // namespace Log
