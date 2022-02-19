#ifndef _MYSQL_CONNECT_H
#define _MYSQL_CONNECT_H

#include <mysql.h>
#include <string>
#include <vector>
#include <chrono>
#include "assert.h"

using namespace std::chrono;

namespace MySql_
{
    inline const char *mySQL_IP = "39.108.131.90";
    inline const char *mySQL_User = "root";
    inline const char *mySQL_Password = "123456";
    inline const char *mySQL_Database = "webserver";
    inline const int mySQL_Port = 3306;

    struct webMes
    {
        
        webMes(const std::vector<std::string>&mes)
        :   id_(::atoi(mes[0].c_str())),
            name_(mes[1]),
            password_(mes[2]),
            signTime_(mes[3]),
            signNum_(::atoi(mes[4].c_str()))
        {

        }
        webMes()=default;

        void set(const std::vector<std::string>&mes)
        {
            assert(mes.size()==5);

            id_=::atoi(mes[0].c_str());
            name_=mes[1];
            password_=mes[2];
            signTime_=mes[3];
            signNum_=::atoi(mes[4].c_str());
        }

        unsigned int id_=0;
        
        std::string name_;
        std::string password_;
        std::string signTime_;
        unsigned int signNum_=0;
    };

    //mysql连接类
    class SqlConn
    {
    public:
        //返回数据库数据的类型。
        using MessType = std::vector<std::vector<std::string>>;

        SqlConn();

        ~SqlConn();

        //搜索数据库
        MessType sendCommend(const std::string &mes);
        bool     sendComWithoutAns(const std::string &mes);

    private:
        MYSQL conns_;
    };

    //处理业务的类
    class SqlMess : public SqlConn
    {
    public:
        //根据名字搜索
        webMes searchByName(const std::string &name);

        //按ID搜索
        webMes searchByName(int);
        
        bool insert(const std::string&,const std::string&,const time_point<system_clock>&);

        //更新密码
        bool updatePassword();

        //插入新数据

        //按名字删除数据
        bool deleteByName(const std::string &name);
    };

}

#endif