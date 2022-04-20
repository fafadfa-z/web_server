#ifndef _MYSQL_BUF_H
#define _MYSQL_BUF_H

#include <string>
#include <mutex>
#include <memory>

#include "LRU_cache.h"

#include "mysql_connect.h"

namespace MySql_
{
    inline int bufSize = 0;
    
    class MySqlBuf
    {
    public:
        MySqlBuf();

        std::shared_ptr<const webMes> searchPlayer(const std::string &);

        bool newPlayer(const std::string &, const std::string &);


    private:

        std::mutex mut_;

        Base::LRU_Cache<std::string,std::shared_ptr<const webMes>> cache_;

        inline static thread_local SqlMess *conn_ = nullptr;
    };
}

#endif