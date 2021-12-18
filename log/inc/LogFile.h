#ifndef LOG_FILE_H
#define LOG_FILE_H

#include <string>
#include <sys/types.h>


namespace log
{
    class LogFile
    {
    public:
        explicit LogFile(const char* fileName);

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