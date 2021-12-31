#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <unistd.h>
#include <string>
#include <mutex>
#include <queue>
#include <memory>


#include <sys/types.h>
#include <sys/socket.h>

class BufferPool;
class TCPConnection;

class Buffer
{

public:
    Buffer();
    ~Buffer();

    bool saveReadable(int fd);

    void readReadbale(std::vector<char>&);

    void sendMessage(std::string&&mes);

    bool sendSendable(int fd);

    bool sendSize(int n);

    void clearRead()
    {
        readIndex1_=0;
        readIndex2_=0;
    }


    static void setPool(std::shared_ptr<BufferPool>& pool){pool_=pool;}

    friend class TCPConnection;

private:


    char* readBuf_;
    char* sendBuf_;

    int ReadBufSize_;
    int SendBufSize_;

    int readIndex1_;
    int readIndex2_;
    
    int sendIndex1_;    // 标记读开始的位置
    int sendIndex2_;    // 标记写开始的位置

    static const int readSize_ = 65535;

    static const int initSize_ = 2048;

    static thread_local  std::shared_ptr<BufferPool>  pool_;
};


#endif