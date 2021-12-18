#ifndef FILE_DESCRIPTOR_H
#define FILE_DESCRIPTOR_H


class fileDescri
{
public:
    fileDescri(int fd)
    :fd_(fd)
    {}

    const int fd()const {return fd_;}

private:

    int fd_;


};



#endif

