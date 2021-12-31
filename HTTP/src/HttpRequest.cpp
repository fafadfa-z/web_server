#include "HttpRequest.h"
#include <algorithm>
#include "logger.h"

#include "server.h"

namespace Http
{

    const char *RN = "\r\n\r\n";

    auto HttpRequest::readMessage(TCPConnection *conn)
    {
        LOG_HTTP << "read http message begin.." << log::end;

        auto [left, right] = conn->buffer();

        int size = 0;

        char* temp;

        int currentSize; //当前传过来的数据数量

        int needSize; //当前还需要的数据量

        std::map<std::string, std::string>::iterator iter;

        switch (state_)
        {
        case empty_:
            left = findMessageHead(conn);

            //如果没有找到Http请求行
            if (left == nullptr)
                return std::pair(badMes, right);

        case needHead_:
            temp = std::search(left, right, RN, RN + 4);

            //如果请求行太长了，，，，
            if (std::distance(left, temp) > maxRequestSize)
                return std::pair(badMes, right);

            state_ = needHead_;

            //如果没有\r\n\r\n 则说明没发完
            if (temp == right)
                return std::pair(MoreMes, left);

            left = getHeader(left, temp);

            state_ = needEntity_;

        case needEntity_:

            //后面没有数据了
            if (iter = headerMap_.find(std::string("Content-Length")); iter == headerMap_.end())
            {
                LOG_HTTP << "read message without entity" << log::end;
                return std::pair(CanDeal, left);
            }
            else
                size =::atoi(iter->second.c_str());

            currentSize = right - left; //当前传过来的数据数量

            needSize = size - entity_.size(); //当前还需要的数据量

            assert(currentSize >= 0);
            assert(needSize >= 0);

            if (needSize < currentSize) //发送来的数据包大于需要的数据包
            {
                entity_.append(left, left + needSize);

                state_ = finish_;

                return std::pair(CanDeal, left + needSize);
            }
            else
            {
                entity_.append(left, left + currentSize);
                return std::pair(MoreMes, right);
            }
        case finish_:

        default: ;

        }
        return std::pair(badMes, right);
    }

    char *HttpRequest::findMessageHead(TCPConnection *conn)
    {
        auto [left, right] = conn->buffer();

        auto index = left;

        while (left <= right - 2)
        {
            index = std::search(left, right, RN, RN + 2); //寻找 \r\n

            if (index == right)
                return nullptr; //没有\r\n 说明是垃圾

            auto ret = getRequest(left, index); //处理请求

            left = index + 2;

            if (ret)
                return left; //如果找到了消息头，则直接返回
        }

        return nullptr;
    }

    bool HttpRequest::getRequest(char *start, char *end)
    {
        auto index = start;

        auto iter = std::find(start, end, ' ');

        if (iter != end && setHttpMode(index, iter))
        {
            index = iter + 1;

            iter = std::find(index, end, ' ');

            if (iter != end)
            {
                query_.append(index, iter);
            }
            if (setHttpVersion(++iter, end))
            {
                return true;
            }
        }
        return false;
    }

    char *HttpRequest::getHeader(char *start, char *end) //检索首部字段。
    {
        LOG_HTTP << "get header begin...." << log::end;

        char *indexStart = start, *indexEnd = start;

        while (indexEnd != end)
        {
            indexEnd = std::search(indexStart, end, RN, RN + 2);

            auto iter = std::find(indexStart, indexEnd, ':');

            if (iter != indexEnd)
                headerMap_[std::string(indexStart, iter)] = std::string(iter + 2, indexEnd);

            indexStart = indexEnd + 2;
        }
        return end + 4;
    }

    bool HttpRequest::setHttpVersion(char *start, char *end)
    {
        auto succeed = (static_cast<int>(end - start) == 8 && std::equal(start, end - 1, "HTTP/1."));

        if (succeed)
        {
            if (*(end - 1) == '1')
            {
                version_ = Http11;
                return true;
            }
            else if (*(end - 1) == '0')
            {
                version_ = Http10;
                return true;
            }
        }
        return false;
    }

    bool HttpRequest::setHttpMode(char *start, char *end)
    {
        std::string message(start, end);

        if (message.find("GET") != -1)
        {
            request_ = Get;
            return true;
        }
        if (message.find("POST") != -1)
        {
            request_ = Get;
            return true;
        }
        return false;
    }

    std::string HttpRequest::displayMode()
    {
        switch (request_)
        {
        case Invalid:
            return "Invalid";
        case Get:
            return "Get";
        case Post:
            return "Post";
        };
        return "";
    }

    std::string HttpRequest::displayVersion()
    {
        switch (version_)
        {
        case Unknown:
            return "Unknown";
        case Http10:
            return "Http10";
        case Http11:
            return "Http11";
        }
        return "";
    }

    void HttpRequest::display()
    {
        LOG_HTTP << "request: " << displayMode() << log::end;
        LOG_HTTP << "version: " << displayVersion() << log::end;

        for (auto temp : headerMap_)
        {
            LOG_HTTP << temp.first << "\t" << temp.second << log::end;
        }
    }

}