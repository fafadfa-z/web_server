#include "tcp_thread_pool_process.h"
#include "tcp_server.h"
#include "tcp_channel.h"
#include "local_message.h"

PoolProcess::PoolProcess()
    :epollFd_(epoll_create(5)),
    timeout_(1000)
    
{
    triggerMod_=Base::LocalMassage::triggerMod();
}

void PoolProcess::operator()()
{
    auto res=::pipe(pipe_);

    weakupChannel_.reset(new Channel(pipe_[0])); 

    assert(res!=-1);
    
    assert(epollFd_ != -1);

    insertToEpoll(weakupChannel_);
    
    channelMap_.insert({pipe_[0],weakupChannel_});

    Channel::setPoolPro(shared_from_this());

    bufPool_.reset(new BufferPool);

    Buffer::setPool(bufPool_);

    while(true)
    {
        distribute();
    }
}

void PoolProcess::distribute()
{
    ::memset(events_,0,sizeof(events_));
    int messageSize = epoll_wait(epollFd_, events_, evenListMax_, timeout_);

    if (messageSize > 0)
    {
        for (int i = 0; i < messageSize; i++)
        {
            MapType::iterator temp;

            temp = channelMap_.find(events_[i].data.fd);

            assert(temp != channelMap_.end());
            
            temp->second->dealEvent(events_[i]);
        }
    }
    else if (messageSize == 0)
    {
        //LOG_INFO << "线程空闲" << Log::end;
    }
    else 
    {   
        LOG_FATAL << "messageSize<0 !" << Log::end;
    }

}

void PoolProcess::insertToEpoll(const std::shared_ptr<Channel>& channel)  //这个函数是接收线程调用的�?
{ 
    epoll_event new_event;
    int fd=channel->fd();

    new_event.events = channel->events();
    
    new_event.data.fd =fd;

    int old_option = fcntl(fd, F_GETFL); //设置为非阻塞�?
    int new_opion = old_option | O_NONBLOCK;

    if(triggerMod_==Base::MOD_ET)
        new_event.events |= EPOLLET;

    fcntl(fd, F_SETFL, new_opion);

    auto res = epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &new_event);

    assert(res == 0);
}

void PoolProcess::removeFd(int fd)          //这个函数是事件处理线程调用的�?
{
    LOG_INFO << "removeFd: " << fd << Log::end;

    auto iter = channelMap_.find(fd);

    assert(iter != channelMap_.end());

    channelMap_.erase(iter);

    epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr);

    ::close(fd);
}

void PoolProcess::changeEvent(int event, int fd)
{
    //LOG_INFO << "Change event  fd=" << fd << " event= " << event << Log::end;
    epoll_event new_event;

    new_event.events = event;

    new_event.data.fd = fd;

    auto res = epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &new_event);

    if (res == -1)
    LOG_FATAL << "Change event error" << Log::end;
}
void PoolProcess::weakup(int mes)
{
    LOG_INFO<<"process weak up!"<<mes<<Log::end;

    int fd=mes;

    auto n = ::write(pipe_[1], &fd, sizeof(int));

    assert(n == sizeof(fd));
}

void PoolProcess::receiveWeakup()
{
    int mes = -1;
    auto n = ::read(pipe_[0], &mes, sizeof(mes));

    assert(n == sizeof(mes));
    assert(mes!=-1);

    LOG_INFO<<"new fd: "<<mes<<Log::end;

    std::shared_ptr<Buffer>buf(std::make_shared<Buffer>());
    std::shared_ptr<Channel>channel(std::make_shared<Channel>(mes,buf));

    channelMap_.insert({mes,channel});

    insertToEpoll(channel);
}

PoolProcess::~PoolProcess()
{
    LOG_DEBUG << "PoolProcess 析构" << Log::end;
}