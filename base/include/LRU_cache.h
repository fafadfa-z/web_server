#pragma once

#include <unordered_map>
#include <optional>
#include <list>

namespace Base
{

    template <typename Key, typename Value>
    class LRU_Cache
    {
    public:
        using List = std::list<std::pair<Key, Value>>;

        using ResultType = std::optional<Value>;

        LRU_Cache(int n = 10)
            : size_(0),
              capacity_(n)
        {
        }

        ResultType get(const Key &key)
        {
            auto iter = hash_.find(key);

            if (iter == hash_.end())
                return std::nullopt;

            auto result = iter->second->second;

            list_.splice(list_.begin(), list_, iter->second);

            return result;
        }

        void put(const Key &key, const Value value)
        {
            auto iter = hash_.find(key);
            if (iter != hash_.end())
            {
                moveToFront(iter->second);
                iter->second->second = value;
            }
            else if (size_ < capacity_)
            {
                list_.emplace_front(key, value);
                size_++;
                hash_[key] = list_.begin();
            }
            else
            {
                auto tempIter = list_.end();
                tempIter--;

                hash_.erase(tempIter->first);
                tempIter->first = key;
                tempIter->second = value;
                hash_[key] = tempIter;

                moveToFront(tempIter);
            }
        }

    private:

        void moveToFront(typename List::iterator node)
        {
            list_.splice(list_.begin(),list_,node);
        }

    
        int size_;
        int capacity_;

        List list_;

        std::unordered_map<Key, typename List::iterator> hash_;
    };

};
