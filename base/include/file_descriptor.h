#ifndef _FILE_DESCRIPTOR_H
#define _FILE_DESCRIPTOR_H

 #include <unistd.h>
 
class fileDescri
{
public:
    fileDescri(int fd)
    :fd_(fd)
    {}
    ~fileDescri()
    {
        ::close(fd);
    }

    const int fd()const {return fd_;}

private:

    int fd_;


};



#endif

