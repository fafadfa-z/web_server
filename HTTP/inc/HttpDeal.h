#ifndef HTTP_DEAL_H
#define HTTP_DEAL_H

#include <memory>
#include <map>

class TCPConnection;
class HtmlFile;

namespace Http
{
    class HttpRequest;
    class WebResources;

    class HttpDeal
    {
    public:
        HttpDeal(HttpRequest& quest,TCPConnection& conn)
        :quest_(quest),conn_(conn)
        {}

        bool dealQuest();

        static void setResource(WebResources*);

    private:
        void sendBadMessage();

        void readEntity(); //读取HTTP报文的实体信息

        void sendFile(const std::string&);  //发送w

    private:
        std::map<std::string,std::string> entityMap_;

        HttpRequest&   quest_;
        
        TCPConnection& conn_;

        static WebResources* resource_;  //澶氱嚎绋嬪湪杩欓噷鍙?鍋氳?诲彇鎿嶄綔锛屽簲璇ヤ笉浼氬嚭鐜拌祫婧愮珵浜夐棶棰橈拷?
    };

}

#endif