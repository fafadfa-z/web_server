#ifndef MYSQL_BUF_H
#define MYSQL_BUF_H

#include <string>
#include "mysqlConn.h"

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