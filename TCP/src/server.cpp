#include "server.h"
#include "logger.h"
#include "assert.h"
#include "channel.h"

#include "task.h"

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
            
}

void TCPServer::beginServer()
{
    LOG_DEBUG << "Server begin......." << log::end;

    pool_ = ThreadPool::init(threadNum_);

    pool_->begin();

    connRecv_ = std::make_shared<EpollConnect>(*listenAddr_,
                                               std::bind(&TCPServer::connNew,this,std::placeholders::_1),
                                               std::bind(&TCPServer::connReadable,this,std::placeholders::_1)); //这地方以后要用工厂模式                               

    connRecv_->begin();
}

// 处理新连接,
void TCPServer::connNew(std::shared_ptr<Channel>channel)
{
    LOG_INFO << "TCPServer::newConnection： " << channel->fd() << "   " << log::end;

    if (connectCallBack_)
    {
        Task task(connectCallBack_,std::move(channel));
        pool_->pushConnect(task);
    }
}

void TCPServer::connReadable(std::shared_ptr<Channel>channel)
{
    LOG_INFO << "connection can be read!" << log::end;

    if (readableCallBack_)
    {
        Task task(readableCallBack_,std::move(channel));
        pool_->pushConnect(task);
    }
}

// void TCPServer::remove+Connection(std::string index)
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