#include "server.h"
#include "logger.h"
#include "assert.h"
#include "channel.h"
typename std::shared_ptr<TCPServer> TCPServer::entity_ = nullptr;

std::shared_ptr<TCPServer> TCPServer::init(TcpAddrPtr listenAddr, int num) //创建服务器对象实例
{
    if (!entity_)
    {
        LOG_DEBUG << "创建 TCPserver" << log::end;
        entity_.reset(new TCPServer(listenAddr, num));
    }
    return entity_;
}

TCPServer::TCPServer(TcpAddrPtr listenAddr, int threadNum)
    : listenAddr_(listenAddr), threadNum_(threadNum)
{
    connRecv_ = std::make_unique<EpollConnect>(*listenAddr,
                                               [this](int fd)
                                               { pool_->pushConnect(fd); }); //这地方以后要用工厂模式                                       
}

void TCPServer::beginServer()
{
    LOG_DEBUG << "Server begin......." << log::end;

    Channel::setServer(this);

    pool_ = ThreadPool::init(threadNum_);

    pool_->begin();

    connRecv_->begin();
}

// 处理新连接,
void TCPServer::connNew(Channel *channel)
{
    LOG_INFO << "TCPServer::newConnection： " << channel->fd() << "   " << log::end;

    TCPConnection connection(channel);

    if (connectCallBack_)
        connectCallBack_(&connection);
}

void TCPServer::connReadable(Channel *channel)
{
    LOG_INFO << "connection can be read!" << log::end;

    TCPConnection connection(channel);

    if (readableCallBack_)
        readableCallBack_(&connection);
}

// void TCPServer::removeConnection(std::string index)
// {
// LOG_DEBUG<<"TCP Server remove connection..."<<log::end;

// std::lock_guard<std::mutex>guard(mut_);
// {
//     auto conn=tcpConnectionMap_.find(index);

//     assert(conn!=tcpConnectionMap_.end());

//     tcpConnectionMap_.erase(conn);
// }
// if(CloseCallBack_) CloseCallBack_();

//  LOG_DEBUG<<"TCP Server remove ok"<<log::end;

// }

TCPServer::~TCPServer()
{
    LOG_DEBUG << "TCP Server析构..." << log::end;
}