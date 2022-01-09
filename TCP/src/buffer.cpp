#include "buffer.h"
#include "assert.h"
#include "logger.h"
#include <errno.h>

#include "buff_pool.h"

thread_local std::shared_ptr<BufferPool> Buffer::pool_ = nullptr;

Buffer::Buffer()
    : readIndex1_(0), readIndex2_(0), sendIndex1_(0), sendIndex2_(0)
{
    assert(pool_ != nullptr);

    ReadBufSize_ = pool_->getBuf(readBuf_, initSize_);
    SendBufSize_ = pool_->getBuf(sendBuf_, initSize_);

    assert(readBuf_ != nullptr);
    assert(sendBuf_ != nullptr);
}

Buffer::~Buffer()
{
    pool_->freeBuf(readBuf_, ReadBufSize_);
    pool_->freeBuf(sendBuf_, SendBufSize_);

    LOG_INFO << "buffer 析构" << Log::end;
}

bool Buffer::saveReadable(int fd)
{
    char buf[readSize_] = {0};

    LOG_INFO << "read begin: fd=" << fd << Log::end;

    auto n = ::recv(fd, buf, readSize_ - 1, 0);

    if (n <= 0)
        return false;

    if (readIndex1_)
        LOG_DEBUG << "writeIndex1_: " << readIndex1_ << Log::end;

    if (readIndex2_ + n > ReadBufSize_)
    {

        int resize = readIndex2_ - readIndex1_ + 1 + n;
        LOG_DEBUG << "read Buf need resize: " << resize << Log::end;

        // readIndex_=pool_->changeBuf(readBuf_,readIndex_,readIndex_+n); //����buffer
        char *temp;
        int preSize = ReadBufSize_;

        ReadBufSize_ = pool_->getBuf(temp, resize);

        assert(temp != nullptr);

        assert(readIndex2_ - readIndex1_ <= ReadBufSize_);

        std::copy(readBuf_ + readIndex1_, readBuf_ + readIndex2_, temp);

        pool_->freeBuf(readBuf_, preSize);

        readIndex2_ -= readIndex1_;
        readIndex1_ = 0;

        readBuf_ = temp;
    }

    std::copy(buf, buf + n, readBuf_ + readIndex2_);

    readIndex2_ += n;

    return true;
}

void Buffer::clear()
{
    readIndex1_ = 0;
    readIndex2_ = 0;
}

void Buffer::clear(char *index)
{    
    if (index == readBuf_ + readIndex2_) //ȫ�����
    {
        readIndex1_ = 0;
        readIndex2_ = 0;
    }
    else 
    {
        readIndex1_=std::distance(readBuf_,index);
        LOG_DEBUG << "��������,����û����� " << Log::end;
    }
}

bool Buffer::sendSendable(int fd)
{
    assert(sendIndex2_ != sendIndex1_);

    int size = sendIndex2_ - sendIndex1_;

    auto n = ::send(fd, sendBuf_ + sendIndex1_, size, 0);

    if (n == size)
    {
        LOG_INFO << "Buffer：发送完成！" << Log::end;
        sendIndex1_ = 0;
        sendIndex2_ = 0;

        return true;
    }
    else
    {
        LOG_DEBUG << "Buffer：需要二次发�????" << Log::end;
        sendIndex1_ += n;
        return false;
    }
}

void Buffer::sendMessage(const std::string &mes) //缓存等待发送的数据
{
    int n = mes.size();
    
    if(n>maxSendSize_) return;
    
    if (sendIndex2_ + n > SendBufSize_)
    {
        int resize = sendIndex2_ - sendIndex1_ + 1 + n;

        LOG_DEBUG << "send Buf need resize: " << resize << Log::end;

        char *temp;
        int preSize = SendBufSize_;

        SendBufSize_ = pool_->getBuf(temp, resize);

        assert(temp != nullptr);

        assert(sendIndex2_ - sendIndex1_ <= SendBufSize_);

        std::copy(sendBuf_ + sendIndex1_, sendBuf_ + sendIndex2_, temp);

        pool_->freeBuf(readBuf_, preSize);

        sendIndex2_ -= sendIndex1_;
        sendIndex1_ = 0;

        sendBuf_ = temp;
    }
        std::copy(mes.begin(), mes.end(), sendBuf_ + sendIndex2_);
        sendIndex2_ += n;
}
