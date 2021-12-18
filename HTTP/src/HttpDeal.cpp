#include "HttpDeal.h"
#include "HttpRequest.h"
#include "HttpMessage.h"
#include "htmlFile.h"
#include "server.h"


namespace Http
{
    WebResources *HttpDeal::resource_ = nullptr;

    void HttpDeal::setResource(WebResources *resource)
    {
        resource_ = resource;
    }

    void HttpDeal::dealQuest()
    {
        if (quest_.request() == Get)
        {
            auto file = resource_->findHtml(quest_.query());

            if (file != nullptr)
            {
                HttpMessage message(Http200);

                message.fillRequestMessage(file->size());

                auto mes=message.dealMessage("ok",file->buf());

                LOG_HTTP<<"send size:"<< mes.size()<<log::end;
            
#if displaySendMess
           LOG_COUT<<"send message: \n"<< mes<<std::endl;        
#endif
                conn_.send(mes);
            }
            else
            {
                HttpMessage message(Http200);
                message.addHeader("Server", "fafadfa");
                auto mes=message.dealMessage("not dound","");
                conn_.send(mes);
                LOG_HTTP<<"error: cannot find request files: "<<quest_.query()<<log::end;
            }
             LOG_HTTP<<"Send message to TCP...."<<log::end;
        }
    }
 
}
