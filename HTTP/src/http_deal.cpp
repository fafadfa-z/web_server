#include "http_deal.h"
#include "http_request.h"
#include "http_message.h"
#include "html_file.h"
#include "tcp_server.h"

#include "mysql_buf.h"


namespace Http
{
    WebResources *HttpDeal::resource_ = nullptr;

    void HttpDeal::setResource(WebResources *resource)
    {
        resource_ = resource;
    }

    bool HttpDeal::dealQuest() //���������������?
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

            if (entityMap_.find("submit") != entityMap_.end()) //登录
            {
                MySql_::MySqlBuf sql;

                auto mes = sql.searchPlayer(entityMap_["username"]);

                if(mes->id_==0) //找不到文件
                {
                    sendFile("/failed.html");
                    return false;
                }

                assert(mes->name_ == entityMap_["username"]);

                std::string pass=entityMap_["pwd"];
                
                if(mes->password_==pass) //登录成功
                {
                    sendFile("/success.html");
                }
                else
                {
                    sendFile("/failed.html");
                }
            }
            else if (entityMap_.find("zhuce") != entityMap_.end()) //注册�?
            {
                LOG_HTTP << "新用户注册!" << Log::end;
                sendFile("/submit.html");
            }
            else if (entityMap_.find("new_player") != entityMap_.end()) //提交注册信息。
            {
                addNewPlayer();
            }
            else
            {
                LOG_HTTP << "excepted..." << Log::end;
            }

            return false;

        default:
            sendBadMessage();
        }

        return false;
    }


    bool HttpDeal::addNewPlayer() //添加新用户
    {
        auto iterName= entityMap_.find("username");
        auto iterPassowrd= entityMap_.find("pwd");
        auto iterRepeat = entityMap_.find("rep_pwd");

        assert(iterName!=entityMap_.end());
        assert(iterPassowrd!=entityMap_.end());
        assert(iterRepeat!=entityMap_.end());

        if(iterPassowrd->second!=iterRepeat->second) 
        {
            sendFile("/failed_player.html");
            return false;
        }

        LOG_DEBUG<<"insert new player"<<Log::end;

        MySql_::MySqlBuf sql;

        auto ret = sql.newPlayer(iterName->second,iterPassowrd->second);

        
        if(ret) //成功了
        {
            sendFile("/success_player.html");
            return true;
        }
        else sendFile("/failed_player.html");

        return false;
    }



    void HttpDeal::sendFile(const std::string&name  )
    {
        std::shared_ptr<const HtmlFile> file;
        if(name=="/")   file=resource_->findHtml("/hellow.html");
        else            file=resource_->findHtml(name);
        
        if(!file)
        {
            LOG_FATAL<<"远程用户请求了不存在的数据"<<Log::end;
            sendBadMessage();
            return;
        }
        HttpMessage message;

        std::string mes;
        
        message.fillRequestMessage(file->size());

        mes = message.dealMessage("ok");

        LOG_HTTP << "send size:" << mes.size() << Log::end;


        conn_.sendWithFile(mes,file->path());
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
        auto mes = message.dealMessage("not found");
        conn_.send(mes);
        LOG_HTTP << "error: cannot find request files: " << quest_.query() << Log::end;
    }

}
