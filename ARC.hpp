#ifndef CACHE_ARC
#define CACHE_ARC

#include <list>
#include <unordered_map>
#include <algorithm>

template <typename T, typename KeyT = int>
struct ContentARC {
    KeyT key;
    T page;
};

template <typename T, typename KeyT = int>
struct Cache_arc_t {

private:
    size_t cache_size_;
    size_t p_;

    std::list<ContentARC<T, KeyT>> T1_;
    std::list<ContentARC<T, KeyT>> T2_;
    std::list<KeyT> B1_;
    std::list<KeyT> B2_;

    using ListIt = typename std::list<ContentARC<T, KeyT>>::iterator;
    using GhostIt = typename std::list<KeyT>::iterator;
    
    std::unordered_map<KeyT, ListIt> T1_map_;
    std::unordered_map<KeyT, ListIt> T2_map_;
    std::unordered_map<KeyT, GhostIt> B1_map_;
    std::unordered_map<KeyT, GhostIt> B2_map_;

public:
    Cache_arc_t(size_t cache_size) : 
        cache_size_(cache_size),
        p_(0) {}

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page);
};

#endif