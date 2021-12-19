#include "poolprocess.h"
#include "LocalMessage.h"
#include "server.h"
#include "channel.h"
#include <algorithm>

PoolProcess::PoolProcess()
{
}

void PoolProcess::operator()()
{
    while (true)
    {
        workLoop();
    }
}

void PoolProcess::workLoop()
{
    std::queue<Task> que;

    que_.swap(que);

    if (que.empty())
    {
        std::this_thread::yield();
    }
    else
    {
        LOG_INFO << "begin deal task.." << log::end;
        auto task = std::move(que.front());
        que.pop();
        task();
    }
}

PoolProcess::~PoolProcess()
{
    LOG_DEBUG << "PoolProcess 析构" << log::end;
}