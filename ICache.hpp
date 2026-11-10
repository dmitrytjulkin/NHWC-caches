#ifndef ICACHE
#define ICACHE

#include <functional>

template <typename T, typename KeyT>
struct ICache {
    virtual ~ICache() = default;
    virtual bool lookup_update(KeyT key, std::function<T(KeyT)> slow_get_page) = 0;
};

#endif