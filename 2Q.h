#ifndef CACHE_2Q
#define CACHE_2Q

#include <list>
#include <unordered_map>
#include <unordered_set>

template <typename T, typename KeyT = int>
struct Content2Q {
    KeyT key;
    T page;
};

template <typename T, typename KeyT = int>
struct Cache_2q_t {

private:
    size_t cache_size_;
    size_t a1_size_;
    size_t am_size_;
    size_t a1out_size_;

    std::list<Content2Q<T, KeyT>> a1_;
    std::list<Content2Q<T, KeyT>> am_;
    std::list<KeyT> a1out_;

    using ListIt = typename std::list<Content2Q<T, KeyT>>::iterator;
    using GhostIt = typename std::list<KeyT>::iterator;
    
    std::unordered_map<KeyT, ListIt> a1_map_;
    std::unordered_map<KeyT, ListIt> am_map_;
    std::unordered_set<KeyT> a1out_set_;

public:
    Cache_2q_t(size_t cache_size) : 
        cache_size_(cache_size),
        a1_size_(cache_size/4),
        am_size_(cache_size - cache_size/4),
        a1out_size_(cache_size/2) {}

    bool a1_full() const {
        return a1_.size() >= a1_size_;
    }

    bool am_full() const {
        return am_.size() >= am_size_;
    }

    bool a1out_full() const {
        return a1out_.size() >= a1out_size_;
    }

    bool in_a1(KeyT key) const;
    bool in_am(KeyT key) const;
    bool in_a1out(KeyT key) const;

    void insert_to_a1(const Content2Q<T, KeyT>& node);
    void insert_to_am(const Content2Q<T, KeyT>& node);
    void insert_to_a1out(KeyT key);

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page);
};

#endif