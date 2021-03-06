#include "http_message.h"
#include "log_thread.h"

#define _r_n "\r\n"

namespace Http
{

    std::string HttpMessage::dealMessage(const std::string& mes)
    {
        std::string ans;

        ans.reserve(mes.size()+25);

        if (version_ == Http11)
            ans += "HTTP/1.1 ";
        else if (version_ == Http10)
            ans += "HTTP/1.0 ";

        ans += std::to_string(static_cast<int>(mod_)); //添加请求编号

        ans += ' ';

        ans += mes;

        ans += _r_n;

        ans += HeaderInfo_;

        ans += _r_n;

        return ans;
    }

    void HttpMessage::addHeader(const std::string &title, const std::string &body)
    {
        HeaderInfo_ += title;

        HeaderInfo_ += ": ";

        HeaderInfo_ += body;

        HeaderInfo_ += _r_n;
    }

}