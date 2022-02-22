#include "log_file.h"

#include "time_point.h"
#include "local_message.h"

namespace Log
{
    LogFile::LogFile()
        : writeBytes_(0),
          maxSizePreFile_(100 * 1024 * 1024), // 100Mb
          fileCount_(1)
    {
        fileName_=Base::LocalMassage::logPath();
        Time::timePoint time(Time::getNowTime());

        std::string name(fileName_);

        name += time.toLogFileName();

        char buf[16]={0};

        getNum(buf);

        name+=buf;
        
        fp_ = ::fopen(name.c_str(), "ae");

        assert(fp_);  //输入的日志保存路径失败

        ::setbuffer(fp_, buffer_, sizeof(buffer_));
    }

    LogFile::~LogFile()
    {
        ::fclose(fp_);
    }

    void LogFile::writeMessage(const char *message, size_t len)
    {
        assert(len>=0);
        size_t written = 0;

        while (written != len)
        {
            size_t remain = len - written;

            auto n = ::fwrite_unlocked(message + written, 1, remain, fp_);

            if (n != remain)
            {
                int err = ferror(fp_);
                if (err)
                {
                    //fprintf(stderr, "AppendFile::append() failed %s\n", strerror_tl(err));
                    break;
                }
            }
            written += n;
        }
        writeBytes_ += written;
        if (static_cast<unsigned long>(writeBytes_) > maxSizePreFile_)
        {
            std::cout<<"change Log file....\n";
            changeFile();
        }
    }
    void LogFile::flush()
    {
        ::fflush(fp_);
    }

    void LogFile::changeFile()     //当文件写入数量足够的时候，换文件
    {
        std::cout << "Change file.....\n";

        Time::timePoint time(Time::getNowTime());

        auto name = (fileName_);
        name += time.toLogFileName();

        char buf[16] = {0};


        getNum(buf);

        name += buf;

        ::fclose(fp_);

        fp_ = ::fopen(name.c_str(), "ae");

        assert(fp_);

        ::setbuffer(fp_, buffer_, sizeof(buffer_));

        writeBytes_=0;
    }

    void LogFile::getNum(char *buf)
    {
        int temp = fileCount_;
        *buf++ = '_';
        while (temp)
        {
            *buf++ = temp % 10+'0';
            temp /= 10;
        }
        *buf++ = '.';
        *buf++ = 'l';
        *buf++ = 'o';
        *buf++ = 'g';
    }

}
