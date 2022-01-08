#include "mysqlBuf.h"
#include "timePoint.h"
namespace MySql_
{
    MySqlBuf::MySqlBuf()
    {
        if(conn_==nullptr) conn_=new SqlMess;
    }

    bool MySqlBuf::newPlayer(const std::string& name,const std::string& password)
    {
        auto time=Time::getNowTime();

        return conn_->insert(name,password,time);

    }

    const webMes MySqlBuf::searchPlayer(const std::string& name)
    {
        return conn_->searchByName(name);
    }
}