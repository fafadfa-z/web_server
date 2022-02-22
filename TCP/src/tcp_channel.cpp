#include "tcp_channel.h"
#include "tcp_thread_pool_process.h"
#include "tcp_server.h"

#include "logger.h"



thread_local std::shared_ptr<PoolProcess> Channel::poolPro_(nullptr);
TCPServer* Channel::server_=nullptr;

void Channel::setPoolPro(std::shared_ptr<PoolProcess> pro)
{
    poolPro_ = pro;
}
void Channel::setServer(TCPServer* server)
{
    server_=server;
}

void Channel::dealEvent(epoll_event &event)
{
    bool isDeal = false;

    if (event.events & POLLOUT)
    {
        LOG_INFO<<"re send begin..."<<Log::end;

        auto flag=buf_->sendSendable(fd_);

        if(flag) 
        {
            bool ret=false;

            if(path_) ret=sendFile();
            
            if(ret==false) disableWrite();
        }

        isDeal = true;
    }
    if ((event.events & POLLIN) || !buf_->empty()) //这个不能放在第一个，如果放在第一个则可能在写操作执行之前关闭端口
    {
        if (fd_==poolPro_->weakUpFd()) // weakup 情况
        {
            poolPro_->receiveWeakup();
        }
        else
        {
            assert(buf_!=nullptr);   
            
            auto res=buf_->saveReadable(fd_);

            if(res)
            {
                server_->connReadable(this);
            }
            else
            {
                Channel::close();
                //关闭channel
            }
        }
        isDeal = true;
    }
    if (event.events & (EPOLLHUP | POLLRDHUP))
    {
        LOG_FATAL << "client want to leave...." << Log::end;
    }
    if (isDeal == false)
    {
        LOG_FATAL << "other message:  " << event.events << Log::end;
    }
    event.events = 0;
    event.data.fd = 0;
}

void Channel::enableWrite()
{
    LOG_INFO << "channel: " << fd_ << "  enable write..." << Log::end;
    
    if(events_ & POLLOUT) //如果之前已经打开了发送
    {  
        return;
    }

    events_ |= POLLOUT;

    poolPro_->changeEvent(events_, fd_);
}

void Channel::disableWrite()
{
   // std::cout << "channel: " << fd_ << "  disable write..." << std::endl;
    assert(events_ & POLLOUT);

    events_ &= 0b1111'1111'1011;

    poolPro_->changeEvent(events_, fd_);
}

void Channel::sendWithFile(const std::string &message, const std::filesystem::path &path)
    {
        send(message);
        path_=path;

        fileFd_=::open(path_->c_str(),O_RDONLY);

        assert(fileFd_>0);
        struct stat fileStat;

        ::fstat(fileFd_,&fileStat);

        fileSize_=fileStat.st_size;
        fileIndex_=0;

    }

bool  Channel::sendFile()
{
    assert(fileIndex_==0);
    fileIndex_=::sendfile(fd_,fileFd_,&fileIndex_,fileSize_);

    if(fileIndex_==fileSize_)
    {
        path_=std::nullopt;
        ::close(fileFd_);
        return false;
    }
    //std::cout<<"一次没发完"<<std::endl;
    return true; //需要重复发送
}

void Channel::close()
{
    poolPro_->removeFd(fd_);
}

Channel::~Channel()
{
    LOG_INFO << "channel 析构" << fd_ << Log::end;
}
