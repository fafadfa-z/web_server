#ifndef _MYSQL_BUF_H
#define _MYSQL_BUF_H

#include <string>
#include "mysql_connect.h"

namespace MySql_
{
    class MySqlBuf
    {
    public:
        MySqlBuf();

        const webMes searchPlayer(const std::string &);

        bool newPlayer(const std::string &, const std::string &);

    private:
        inline static thread_local SqlMess *conn_ = nullptr;
    };
}

#endif