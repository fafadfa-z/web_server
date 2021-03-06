#include "tcp_buff_pool.h"
#include "algorithm"
#include "local_message.h"
#include "logger.h"

#include "string.h"
static constexpr int buf2kNum = 10'000;
static constexpr int buf4kNum = 1'000;
static constexpr int buf8kNum = 500;
static constexpr int buf16kNum = 200;


static constexpr int num_2k=2*1024;
static constexpr int num_4k=4*1024;
static constexpr int num_8k=8*1024;
static constexpr int num_16k=16*1024;

BufferPool::BufferPool()
{
    localBuf_.threadId_ = getTid();
    /*--------------------------�����ڴ�---------------------*/
    localBuf_.buf2k_ = (char *)::malloc(buf2kNum * 2 * 1024);

    assert(localBuf_.buf2k_ != nullptr);

    localBuf_.buf4k_ = (char *)::malloc(buf4kNum * 2 * 1024);

    assert(localBuf_.buf4k_ != nullptr);

    localBuf_.buf8k_ = (char *)::malloc(buf8kNum * 2 * 1024);

    assert(localBuf_.buf8k_ != nullptr);

    localBuf_.buf16k_ = (char *)::malloc(buf16kNum * 2 * 1024);

    assert(localBuf_.buf16k_ != nullptr);

    /*--------------------------ѹջ--------------------------*/

    for (int j = 0; j < buf2kNum; j++)
        localBuf_.st2k_.push(localBuf_.buf2k_ + j*num_2k);

    for (int j = 0; j < buf4kNum; j++)
        localBuf_.st4k_.push(localBuf_.buf4k_ + j*num_4k);

    for (int j = 0; j < buf8kNum; j++)
        localBuf_.st8k_.push(localBuf_.buf8k_ + j*num_8k);

    for (int j = 0; j < buf16kNum; j++)
        localBuf_.st16k_.push(localBuf_.buf16k_ +j*num_16k);
}

int BufferPool::getBuf(char *&buf, int size)
{
    int ans;

    if (size <= num_2k)
    {
        if (localBuf_.st2k_.empty())
        {
            buf = nullptr;
            return 0;
        }

        ans = num_2k;

        buf = localBuf_.st2k_.top();
        localBuf_.st2k_.pop();
        localBuf_.used2k_++;
    }
    else if (size <= num_4k)
    {
        if (localBuf_.st4k_.empty())
        {
            buf = nullptr;
            return 0;
        }

        ans = num_4k;
        buf = localBuf_.st4k_.top();
        localBuf_.st4k_.pop();
        localBuf_.used4k_++;
    }
    else if (size <= num_8k)
    {
        if (localBuf_.st8k_.empty())
        {
            buf = nullptr;
            return 0;
        }
        ans = num_8k;

        buf = localBuf_.st8k_.top();
        localBuf_.st8k_.pop();
        localBuf_.used8k_++;
    }
    else if (size <= num_16k)
    {
        if (localBuf_.st16k_.empty())
        {
            buf = nullptr;
            return 0;
        }

        ans = num_16k;

        buf = localBuf_.st16k_.top();
        localBuf_.st16k_.pop();
        localBuf_.used16k_++;
    }
    LOG_INFO<<"get buffer size: "<<size<<"index: "<<buf<<Log::end;
    
    return ans;
}
void BufferPool::freeBuf(char *buf, int size)
{
    //::memset(buf, 0, size);

    LOG_INFO<<"free buffer size: "<<size<<"index: "<<buf<<Log::end;

    if (size <= 2048)
    {
        localBuf_.st2k_.push(buf);

        localBuf_.used2k_--;
    }
    else if (size <= 4096)
    {
        localBuf_.st4k_.push(buf);

        localBuf_.used4k_--;
    }
    else if (size <= 8192)
    {
        localBuf_.st8k_.push(buf);

        localBuf_.used8k_--;
    }
    else if (size <= 16384)
    {
        localBuf_.st16k_.push(buf);

        localBuf_.used16k_--;
    }
}

BufferPool::~BufferPool()
{
    ::free(localBuf_.buf2k_);
    ::free(localBuf_.buf4k_);
    ::free(localBuf_.buf8k_);
    ::free(localBuf_.buf16k_);
}