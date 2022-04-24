#include "tcp_server.h"
#include "logger.h"
#include "tcp_channel.h"
#include "timmer.h"

typename std::shared_ptr<TCPServer> TCPServer::entity_ = nullptr;

std::shared_ptr<TCPServer> TCPServer::init(int num, bool disMes) //创建服务器对象实例
{
    if (!entity_)
    {
        LOG_DEBUG << "创建 TCPserver" << Log::end;
        entity_.reset(new TCPServer(num, disMes));
    }
    return entity_;
}

TCPServer::TCPServer(int threadNum, bool disMes)
    : threadNum_(threadNum),
      recordFlag_(false),
      disMes_(disMes)
{
    connRecv_ = std::make_unique<EpollConnect>([this](int fd, TimePoint point)
                                               { connNewFd(fd, point); }); //这地方以后要用工厂模式
}

void TCPServer::beginServer()
{
    LOG_DEBUG << "Server begin......." << Log::end;

    Channel::setServer(this);

    pool_ = ThreadPool::init(threadNum_);

    pool_->begin();

    connRecv_->begin();
}

void TCPServer::connNewFd(int fd, TimePoint piont)
{
    if (disMes_)  //如果需要输出统计信息
    {
        if (!recordFlag_) // 开始记录
        {
            auto tim = Time::Timmer::init(10);

            tim->addOnceTask([this](long time)
                             { disMes(time); },
                             5000); //

            recordFlag_ = true;
        }
        assert(fdMap_.find(fd) == fdMap_.end());

        {
            std::lock_guard guard(mut_);
            fdMap_.insert({fd, piont});
            currentNum_++;
            maxNum_ = std::max(maxNum_, currentNum_);
        }
    }

    pool_->pushConnect(fd);
}

void TCPServer::connDelFd(int fd) //连接断开时的统计信息
{
    assert(fdMap_.find(fd) != fdMap_.end());

    std::lock_guard guard(mut_);

    auto iter = fdMap_.find(fd);

    auto nowTime = std::chrono::system_clock::now();

    auto timeTemp = std::chrono::duration_cast<std::chrono::microseconds>(nowTime - iter->second);

    totalTime_ += timeTemp.count();

    fdMap_.erase(iter);

    currentNum_--;
    recordNum_++;
}

void TCPServer::disMes(long time) // 通过定时器循环调动的统计信息函数
{
    double recordNum;
    long maxNum;
    double totalTime;

    time /= 1000; //单位转换为秒

    {
        std::lock_guard guard(mut_);

        recordNum = static_cast<double>(recordNum_) / time;
        maxNum = maxNum_;
        totalTime = static_cast<double>(totalTime_) / 1000;

        recordNum_ = 0;
        maxNum_ = 0;
        currentNum_ = 0;

        recordFlag_ = false;
    }

    LOG_COUT << "Statistical: 统计时间:" << (double)time / 1000 << "ms\n";

    LOG_COUT << "QPS: " << recordNum << " 任务/s\n";

    LOG_COUT << "最大同时连接数: " << maxNum << "\n";

    LOG_COUT << "平均单次处理时间: " << totalTime << " ms" << std::endl;
}

// 处理新连接,
void TCPServer::connNew(Channel *channel)
{
    LOG_INFO << "TCPServer::newConnection: " << channel->fd() << "   " << Log::end;

    TCPConnection connection(channel);

    if (connectCallBack_)
        connectCallBack_(&connection);
}

void TCPServer::connReadable(Channel *channel)
{
    LOG_INFO << "connection can be read!" << Log::end;

    TCPConnection connection(channel);

    if (readableCallBack_)
        readableCallBack_(&connection);
}

TCPServer::~TCPServer()
{
    LOG_DEBUG << "TCP Server析构..." << Log::end;
}