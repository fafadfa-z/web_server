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

    void clear(char* index);  //将前面的数据移出buffer
    void clear();             //将所有数据移除

    void sendMessage(std::string&&mes);

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
    
    int sendIndex1_;    // 鏍囪璇诲紑濮嬬殑浣嶇疆
    int sendIndex2_;    // 鏍囪鍐欏紑濮嬬殑浣嶇疆

    static const int readSize_ = 65535;

    static const int initSize_ = 2048;

    static thread_local  std::shared_ptr<BufferPool>  pool_;
};


#endif