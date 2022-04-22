#ifndef _LOCAL_MASSAGE_H
#define _LOCAL_MASSAGE_H

#include <string>
#include <filesystem>
#include <map>

#include <sys/syscall.h>
#include <unistd.h>
#include "file_read.h"

extern thread_local int thread_tid;

inline int getTid()
{
    if (__builtin_expect(thread_tid == 0, 0))
    {
        thread_tid = static_cast<int>(::syscall(SYS_gettid));
    }
    return thread_tid;
}
namespace Base
{
    inline const int MOD_LT=0;
    inline const int MOD_ET=1;


    class LocalMassage : public FileRead
    {
    public:
        static LocalMassage *readConfig(const std::filesystem::path &);

        ~LocalMassage();

        static LocalMassage* entity(){return entity_;}

        static int port() {return linstenPort_;}

        static std::string& htmlPath() {return htmlPath_;}

        static std::string& logPath() {return logPath_;}
        static int triggerMod() {return triggerMod_;}


        std::string configRead(const char*);
        std::string configRead(const std::string& str);

    private:

        void prinfMes();

        LocalMassage(const std::filesystem::path &);

        std::filesystem::path configFile_;

        std::map<std::string,std::string>confMap_;

        static inline int linstenPort_=-1;
        static inline std::string htmlPath_;
        static inline std::string logPath_;
        static inline int triggerMod_= MOD_LT;

        static inline LocalMassage *entity_; //单例实体
    };

}

#endif
