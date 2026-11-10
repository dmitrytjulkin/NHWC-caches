#ifndef COMMON
#define COMMON

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

#include "2Q/2Q.hpp"
#include "ARC/ARC.hpp"
#include "LFU/LFU.hpp"
#include "ICache.hpp"

template <typename T, typename KeyT>
struct CacheFactory {

private:    
    std::unordered_map<std::string, std::function<std::unique_ptr<ICache<T, KeyT>>(int)>> table_;

public:
    CacheFactory() {
        table_["ARC"] = [](int size) {
            return std::make_unique<Cache_arc_t<T, KeyT>>(size);
        };

        table_["2Q"] = [](int size) {
            return std::make_unique<Cache_2q_t<T, KeyT>>(size);
        };

        table_["LFU"] = [](int size) {
            return std::make_unique<LFU_cache_t<T, KeyT>>(size);
        };
    }

    std::unique_ptr<ICache<T, KeyT>> create(std::string type, int size) const {
        auto it = table_.find(type);
        if (it != table_.end())
            return it->second(size);

        return nullptr; 
    }
};

#endif