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

    std::list<ContentARC<T, KeyT>> t1_;
    std::list<ContentARC<T, KeyT>> t2_;
    std::list<KeyT> b1_;
    std::list<KeyT> b2_;

    using ListIt = typename std::list<ContentARC<T, KeyT>>::iterator;
    using GhostIt = typename std::list<KeyT>::iterator;
    
    std::unordered_map<KeyT, ListIt> t1_map_;
    std::unordered_map<KeyT, ListIt> t2_map_;
    std::unordered_map<KeyT, GhostIt> b1_map_;
    std::unordered_map<KeyT, GhostIt> b2_map_;

public:
    Cache_arc_t(size_t cache_size) : 
        cache_size_(cache_size),
        p_(0) {}

    bool in_t1(KeyT key) const {
        return t1_map_.find(key) != t1_map_.end();
    }

    bool in_t2(KeyT key) const {
        return t2_map_.find(key) != t2_map_.end();
    }

    bool in_b1(KeyT key) const {
        return b1_map_.find(key) != b1_map_.end();
    }

    bool in_b2(KeyT key) const {
        return b2_map_.find(key) != b2_map_.end();
    }

    bool cache_full() const {
        return (t1_.size() + t2_.size()) >= cache_size_;
    }

    void insert_to_b1(KeyT key);
    void insert_to_b2(KeyT key);
    void replace(KeyT key);
    void insert_to_t1(const ContentARC<T, KeyT>& node);
    void insert_to_t2(const ContentARC<T, KeyT>& node);

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page);
};

#endif