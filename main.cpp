#include <iostream>
#include <vector>
#include <optional>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "get_params.h"

template <typename T, typename KeyT = int>
struct cache_2q_t {

private:
    size_t cache_size_;
    size_t a1_size_;
    size_t am_size_;
    size_t a1out_size_;

    std::list<T> a1_;
    std::list<T> am_;
    std::list<KeyT> a1out_;

    using ListIt = typename std::list<T>::iterator;
    using GhostIt = typename std::list<KeyT>::iterator;
    
    std::unordered_map<KeyT, ListIt> a1_map_;
    std::unordered_map<KeyT, ListIt> am_map_;
    std::unordered_set<KeyT> a1out_set_;

public:
    cache_2q_t(size_t cache_size) : 
        cache_size_(cache_size),
        a1_size_(cache_size/4),
        am_size_(cache_size_ - a1_size_),
        a1out_size_(cache_size/2) {}

    bool in_a1(KeyT key) const;
    bool in_am(KeyT key) const;
    bool in_a1out(KeyT key) const;

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page);
};

template <typename T, typename KeyT>
bool cache_2q_t<T, KeyT>::in_a1(KeyT key) const {
    return a1_map_.find(key) != a1_map_.end();
}

template <typename T, typename KeyT>
bool cache_2q_t<T, KeyT>::in_am(KeyT key) const {
    return am_map_.find(key) != am_map_.end();
}

template <typename T, typename KeyT>
bool cache_2q_t<T, KeyT>::in_a1out(KeyT key) const {
    return a1out_set_.contains(key);
}

int main()
{
    std::optional<InputData> input = Reading();

    if (input == std::nullopt)
        return 1;

    return 0;
}