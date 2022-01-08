#include "HttpDeal.h"
#include "HttpRequest.h"
#include "HttpMessage.h"
#include "htmlFile.h"
#include "server.h"
#include "assert.h"
#include "mysqlBuf.h"


namespace Http
{
    WebResources *HttpDeal::resource_ = nullptr;

    void HttpDeal::setResource(WebResources *resource)
    {
        resource_ = resource;
    }

    bool HttpDeal::dealQuest() //���������������
    {
        auto& fileName = quest_.query();

        switch (quest_.request())
        {
        case Get:
        {
            sendFile(fileName);

            return true;
        }
        case Post:

            readEntity(); //处理实体部分
            // for (auto &[a, b] : entityMap_)
            // {
            //     std::cout << std::endl
            //               << a << "   " << b << std::endl;
            // }

            if (entityMap_.find("submit") != entityMap_.end()) //登录
            {
                MySql_::MySqlBuf sql;

                auto mes = sql.searchPlayer(entityMap_["username"]);

                assert(mes.name_ == entityMap_["username"]);

                std::string pass="";
                if(mes.password_==pass) //登录成功
                {
                    sendFile("success");
                }
                else
                {
                    sendFile("failed");
                }

            }
            else if (entityMap_.find("zhuce") != entityMap_.end()) //注册。
            {
                LOG_HTTP << "新的注册用户!" << Log::end;
                sendFile(fileName);

            }
            else
            {
                LOG_HTTP << "excepted..." << Log::end;
            }

            return true;

        default:
            sendBadMessage();
        }

        return false;
    }
    void HttpDeal::sendFile(const std::string&name  )
    {
        auto file = resource_->findHtml(name);
        if(!file)
        {
            LOG_FATAL<<"No file to send!"<<Log::end;
            sendBadMessage();
            return;
        }
        HttpMessage message;

        std::string mes;
        
        message.fillRequestMessage(file->size());

        mes = message.dealMessage("ok", file->buf());

        LOG_HTTP << "send size:" << mes.size() << Log::end;

        conn_.send(mes);
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

                //std::cout<<"Insert: "<<std::string(left, equal)<<"    "<<std::string(equal + 1, temp)<<std::endl;

                left = temp + 1;
            }
            temp++;
        }
        auto equal = std::find(left, right, '=');

        assert(equal != right);

        entityMap_.insert({std::string(left, equal), std::string(equal + 1, right)});
        //std::cout<<"Insert: "<<std::string(left, equal)<<"    "<<std::string(equal + 1, right)<<std::endl;
    }

    void HttpDeal::sendBadMessage()
    {
        HttpMessage message(Http404);
        message.addHeader("Server", "fafadfa");
        auto mes = message.dealMessage("not found", "");
        conn_.send(mes);
        LOG_HTTP << "error: cannot find request files: " << quest_.query() << Log::end;
    }

}
