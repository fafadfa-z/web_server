#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <unistd.h>
#include <string>
#include <mutex>
#include <queue>


#include <sys/types.h>
#include <sys/socket.h>


class Buffer
{

public:
    Buffer();
    ~Buffer();

    bool saveReadable(int fd);

    void  readReadbale(std::vector<char>&);

    void sendMessage(std::string&&mes);

    bool sendSendable(int fd);


    bool sendSize(int n);

private:

    int readIndex_;

    std::vector<char> readBuf_;

    std::vector<char> sendBuf_;

    int sendIndex1_;    // 标记读开始的位置
    int sendIndex2_;    // 标记写开始的位置

    std::mutex bufGuard_;

    static const int readSize_ = 65535;

    static const int initSize_ = 2048;

};


#endif