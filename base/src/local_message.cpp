#include "local_message.h"
#include "logger.h"


LocalMassage*LocalMassage::entity_=nullptr;




thread_local int thread_tid=0;



LocalMassage *LocalMassage::init(const std::string &settingPath)
{
    if (!entity_)
    {
        LOG_DEBUG<<"init local message..."<<Log::end;
        entity_ = new LocalMassage(settingPath);
    }
    return entity_;
}


LocalMassage::LocalMassage(const std::string&)
 {

 }

LocalMassage::~LocalMassage()
{


}