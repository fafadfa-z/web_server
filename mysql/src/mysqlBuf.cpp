#include "mysql_buf.h"
#include "time_point.h"

#include <cassert>

namespace MySql_
{
    MySqlBuf::MySqlBuf()
        : cache_(20)
    {
        if (conn_ == nullptr)
            conn_ = new SqlMess;
    }

    bool MySqlBuf::newPlayer(const std::string &name, const std::string &password)
    {
        auto time = Time::getNowTime();

        return conn_->insert(name, password, time);
    }

    std::shared_ptr<const webMes> MySqlBuf::searchPlayer(const std::string &name)
    {
        assert(conn_ != nullptr);

        std::optional<std::shared_ptr<const webMes>> res;

        {
            std::lock_guard guard(mut_);

            res = cache_.get(name); // 先查缓存
        }

        if (res != std::nullopt)
        {
            return res.value();
        }

        auto result = conn_->searchByName(name);

        {
            std::lock_guard guard(mut_);

            cache_.put(name, result);
        }

        return result;
    }
}