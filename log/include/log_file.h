#ifndef _LOG_FILE_H
#define _LOG_FILE_H

#include <string>
#include <sys/types.h>
#include <assert.h>
#include <string.h>
#include <iostream>

namespace Log
{
    class LogFile
    {
    public:
        LogFile();

        ~LogFile();

        void writeMessage(const char* message,const size_t len);

        off_t writtenBytes_() {return writeBytes_;}

        void flush();

    private:

        void changeFile();

        void getNum(char*);

        FILE* fp_;
        char buffer_[64*1024]; //64kb
        off_t writeBytes_;
        const unsigned long maxSizePreFile_;
        int fileCount_;  
        std::string   fileName_;  
    };

}

#endif