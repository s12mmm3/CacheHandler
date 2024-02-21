#ifndef CACHEHANDLER_H
#define CACHEHANDLER_H

#include <functional>
#include <iostream>
#include <chrono>
#include <map>

#define DEFINE_VALUE(type, valueName, defaultValue) \
public: \
    type valueName() const { return m_##valueName; } \
    void set_##valueName(type valueName) { \
        if(valueName == m_##valueName) { \
            return; \
    } \
        m_##valueName = valueName; \
} \
    private: \
    type m_##valueName = defaultValue; \
    public: \

template<typename R, typename ...A>
class CacheHandler
{
    using KeyType = std::tuple<A...>;
    using ValueType = std::tuple<long long, R>;
public:
    CacheHandler(std::function<R(A...)> func,
                  int timeout = -1,
                  int maxSize = -1)
        : m_func(func), m_timeout(timeout), m_maxSize(maxSize)
    {

    }

    R value(A... args)
    {
        R result;
        if (!enabled()) {
            result = m_func(args...);
        }
        else if (m_cache.find(KeyType(args...)) != m_cache.end())
        {
            ValueType data = m_cache.at(KeyType(args...));
            long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                                   std::chrono::system_clock::now().time_since_epoch()).count() - std::get<0>(data);
            if (elapsedTime - timeout() <= 0) {
                std::cout << "Cache Hit" << std::endl;
                result = std::get<1>(data);
            }
            else {
                std::cout << "Cache Hit But Timeout" << std::endl;
                result = insert(args...);
            }
        }
        else
        {
            std::cout << "Cache Not Hit" << std::endl;
            if (m_cache.size() == maxSize()) {
                // First remove timeout item
                removeTimeout(false);
                if (m_cache.size() == maxSize()) {
                    // Second remove the latest item
                    m_cache.erase(m_cache.begin());
                }
            }
            result = insert(args...);
        }
        return result;
    }

    std::map<KeyType, ValueType> cache() {
        return m_cache;
    }

    int size() {
        return m_cache.size();
    }

    void clear() {
        return m_cache.clear();
    }

    void erase(A... args) {
        m_cache.erase(KeyType(args...));
    }

    void removeTimeout(bool isAll = true) {
        for (auto i = m_cache.begin(); i != m_cache.end(); i++) {
            auto data = i->second;
            long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                                        std::chrono::system_clock::now().time_since_epoch()).count() - std::get<0>(data);
            if (elapsedTime - timeout() <= 0) {
                m_cache.erase(i);
                if (!isAll) {
                    break;
                }
            }
        }
    }

private:
    R insert(A... args) {
        R newData = m_func(args...);
        auto data = ValueType(std::chrono::duration_cast<std::chrono::milliseconds>(
                                  std::chrono::system_clock::now().time_since_epoch()).count(), newData);
        auto key = KeyType(args...);
        m_cache.erase(key);
        m_cache.insert(std::pair<KeyType, ValueType>(key, data));
        return newData;
    }

    std::function<R(A...)> m_func;
    std::map<KeyType, ValueType> m_cache;

    DEFINE_VALUE(long long, timeout, -1)
    DEFINE_VALUE(int, maxSize, -1)
    DEFINE_VALUE(bool, enabled, true)
};

#endif // CACHEHANDLER_H
