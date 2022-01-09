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

    void clear(char* index);  //��ǰ��������Ƴ�buffer
    void clear();             //�����������Ƴ�

    void sendMessage(const std::string& mes);

    bool sendSendable(int fd);

    bool sendSize(int n);

    bool empty(){return readIndex1_==readIndex2_;}

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

    static const int maxSendSize_=16384; //单次发送的最大限制。
};


#endif