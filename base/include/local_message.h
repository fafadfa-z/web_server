#ifndef _LOCAL_MASSAGE_H
#define _LOCAL_MASSAGE_H

#include <string>
#include <sys/syscall.h> 
#include <unistd.h> 


extern thread_local int thread_tid;

inline int getTid()
{
    if(__builtin_expect(thread_tid==0,0))
    {
        thread_tid=static_cast<int>(::syscall(SYS_gettid));
    }
    return thread_tid;
}

class LocalMassage
{
public:
    static LocalMassage* init(const std::string& );

    ~LocalMassage();   

private:
    LocalMassage(const std::string&);

    static LocalMassage* entity_;  //单例实体
};



#endif

