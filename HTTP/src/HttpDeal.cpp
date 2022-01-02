#include "HttpDeal.h"
#include "HttpRequest.h"
#include "HttpMessage.h"
#include "htmlFile.h"
#include "server.h"
#include "assert.h"

namespace Http
{
    WebResources *HttpDeal::resource_ = nullptr;

    void HttpDeal::setResource(WebResources *resource)
    {
        resource_ = resource;
    }

    bool HttpDeal::dealQuest() //处理请求的主函数
    {

        auto file = resource_->findHtml(quest_.query());

        HttpMessage message;
        std::string mes;

        if (file == nullptr)
        {
            sendBadMessage();
            return false;
        }
        switch (quest_.request())
        {
        case Get:
            message.fillRequestMessage(file->size());

            mes = message.dealMessage("ok", file->buf());

            LOG_HTTP << "send size:" << mes.size() << log::end;

            conn_.send(mes);

            return true;
        case Post:

            readEntity(); //提取数据

            if (entityMap_.find("register")!=entityMap_.end())
            {

            }
            else if (entityMap_.find("sign in")!=entityMap_.end())
            {
            }

            return true;

        default:
            sendBadMessage();
        }

        return false;
    }
    void HttpDeal::readEntity()
    {
        const std::string &mes = quest_.entity();

        auto left = mes.begin(), right = mes.end();
        auto temp = left;

        while (temp != right)
        {
            if (*temp == '&')
            {
                auto equal = std::find(left, temp, '=');

                assert(equal != temp);

                entityMap_.insert({std::string(left, equal), std::string(equal + 1, temp)});

                left = temp + 1;
            }
            temp++;
        }
        auto equal = std::find(left, right, '=');

        assert(equal != right);

        entityMap_.insert({std::string(left, equal), std::string(equal + 1, right)});
    }

    void HttpDeal::sendBadMessage()
    {
        HttpMessage message(Http404);
        message.addHeader("Server", "fafadfa");
        auto mes = message.dealMessage("not found", "");
        conn_.send(mes);
        LOG_HTTP << "error: cannot find request files: " << quest_.query() << log::end;
    }

}
