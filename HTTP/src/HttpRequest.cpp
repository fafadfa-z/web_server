#include "HttpRequest.h"
#include <algorithm>
#include "logger.h"

#include "server.h"


namespace Http
{

    const char *RN = "\r\n";

    bool HttpRequest::readMessage(TCPConnectionPtr conn)
    {
        std::vector<char>buf; 
        conn->readBuf(buf);

        LOG_HTTP<<"read http message begin.."<<log::end;

        auto index = std::search(buf.begin(), buf.end(), RN, RN + 2); //寻找 \r\n

        if (index == buf.end())
            return false;

        if (getRequest(&buf[0], &*index)) //处理请求行
        {
            getHeader(&*(index + 2), &*buf.end()); // 处理首部行
            LOG_HTTP<<"read http message finish!"<<log::end;
        }
        else return false;

        return true;
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
                query_ += std::string(index, iter);
            }

            if (setHttpVersion(++iter, end))
            {
                return true;
            }
        }
        else
        {
            return false;
        }
        return false;
    }

    void HttpRequest::getHeader(char *start, char *end)
    {
        LOG_HTTP << "get header begin...." << log::end;

        char *indexStart = start, *indexEnd = start;

        while (indexEnd != end)
        {
            indexEnd = std::search(indexStart, end, RN, RN + 2);

            auto iter = std::find(indexStart, indexEnd, ':');

            if (iter != indexEnd)
                headerMap_[std::string(indexStart, iter)] = std::string(iter + 2, indexEnd);

            indexStart = indexEnd + 2; //这里在最后一次指针溢出了，但是并没有被使用，所以没有问题
        }
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

        if (message == "GET")
        {
            request_ = Get;
            return true;
        }
        if (message == "POST")
        {
            request_ = Post;
            return true;
        }
        if (message == "HEAD")
        {
            request_ = Head;
            return true;
        }
        if (message == "KPUT")
        {
            request_ = Put;
            return true;
        }
        if (message == "kDelete")
        {
            request_ = Delete;
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
        case Head:
            return "Head";
        case Put:
            return "Put";
        case Delete:
            return "Delete";
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