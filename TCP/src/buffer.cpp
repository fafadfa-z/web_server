#include "buffer.h"
#include "assert.h"
#include "logger.h"
#include <errno.h>

Buffer::Buffer()
    : readBuf_(initSize_), sendBuf_(initSize_), readIndex_(0), sendIndex1_(0), sendIndex2_(0)
{
}

Buffer::~Buffer()
{
    LOG_INFO << "buffer 析构"<<log::end;
}

bool Buffer::saveReadable(int fd)
{
    char buf[readSize_] = {0};

    LOG_INFO << "read begin: fd=" << fd << log::end;

    auto n = ::recv(fd, buf, readSize_ - 1, 0);

    if (n <= 0)
        return false;

    if (readIndex_)
        LOG_DEBUG << "writeIndex_: " << readIndex_ << log::end;

    if (readIndex_ + n > readBuf_.size())
    {
        LOG_DEBUG << "read Buf need resize..." << log::end;
        readBuf_.resize(readIndex_ + n);
    }

    std::move(buf, buf + n, readBuf_.begin() + readIndex_);

    readIndex_ += n;

    return true;
}

void Buffer::readReadbale(std::vector<char> &vec)
{
    vec.assign(readBuf_.begin(),readBuf_.begin()+readIndex_);

    readIndex_ = 0;
}

bool Buffer::sendSendable(int fd)
{
    assert(sendIndex2_ != sendIndex1_);

    int size = sendIndex2_ - sendIndex1_;

    auto n = ::send(fd, &*sendBuf_.begin() + sendIndex1_, size, 0);

    LOG_INFO <<"fafadfa"<<std::string(sendBuf_.begin(),sendBuf_.end()) << log::end;

    if (n == size)
    {
        LOG_INFO << "Buffer：发送完成！" << log::end;
        sendIndex1_ = 0;
        sendIndex2_ = 0;

        return true;
    }
    else
    {
        LOG_DEBUG << "Buffer：需要二次发送" << log::end;
        sendIndex1_ += n;
        return false;
    }
}

void Buffer::sendMessage(std::string &&mes) //缓存等待发送的数据
{
    int size = mes.size();
    if (sendIndex2_ + size > sendBuf_.size())
    {
        LOG_DEBUG<<"Buffer 扩容.. size= "<<size<<" buf size: "<<sendBuf_.size()<<log::end;

        std::vector<char> temp(sendIndex2_ + size); //先分配内存，手动扩容

        if (sendIndex1_ != 0)
            std::move(sendBuf_.begin() + sendIndex1_,
                      sendBuf_.begin() + sendIndex2_,
                      temp.begin());

        std::move(mes.begin(), mes.end(), temp.begin() + sendIndex2_ - sendIndex1_);

        std::swap(temp, sendBuf_);

        sendIndex1_ = 0;
        sendIndex2_ = sendIndex2_ - sendIndex1_ + size;

        temp.clear();
    }
    else
    {
        std::move(mes.begin(), mes.end(), sendBuf_.begin() + sendIndex2_);
        sendIndex2_ += size;
    }
}
