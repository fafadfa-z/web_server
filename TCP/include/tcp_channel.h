#ifndef _TCP_CHANNEL_H
#define _TCP_CHANNEL_H

#include <memory>
#include <string>
#include <functional>
#include <mutex>
#include <vector>
#include <optional>
#include <filesystem>
#include <chrono>
#include <assert.h>

#include <sys/epoll.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sendfile.h>


#include "tcp_buffer.h"
#include "time_point.h"

class PoolProcess;
class TCPServer;


class Channel : public std::enable_shared_from_this<Channel>
{
    using CallBackFunction = std::function<void()>;

public:
    Channel(int fd, std::shared_ptr<Buffer> buf = nullptr)
        : fd_(fd),
          events_(EPOLLIN | EPOLLPRI),
          fileFd_(0),
          fileSize_(0),
          fileIndex_(0),
          path_(std::nullopt),
          timeCost_("channel: "),
          buf_(buf)
    {
    }

    ~Channel();

    // void readAll(std::vector<char> &);

    void send(const std::string &message);


    void sendWithFile(const std::string &, const std::filesystem::path &);

    void dealEvent(epoll_event &event); //事件提取

    const int events() const { return events_; }

    void enableWrite();

    Buffer *buf() { return buf_.get(); };

    static void setPoolPro(std::shared_ptr<PoolProcess> pro);
    static void setServer(TCPServer *server);

    int fd() const { return fd_; }

private:
    void disableWrite();
    bool sendFile();

    void close();

    int fd_;
    int events_;
    int fileFd_;

    int64_t fileSize_;
    off_t fileIndex_;

    std::optional<std::filesystem::path> path_;
    
    Time::TimeRecord timeCost_; // 记录占用时间

    std::shared_ptr<Buffer> buf_;

private:
    static thread_local std::shared_ptr<PoolProcess> poolPro_;
    static TCPServer *server_;
};

#endif