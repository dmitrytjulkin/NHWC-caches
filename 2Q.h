#ifndef CACHE_2Q
#define CACHE_2Q

#include <list>
#include <unordered_map>
#include <algorithm>

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
    std::unordered_map<KeyT, GhostIt> a1out_map_;

public:
    Cache_2q_t(size_t cache_size) : 
        cache_size_(cache_size),
        a1_size_(std::max(size_t{1},  cache_size/4)),
        am_size_(cache_size - cache_size/4),
        a1out_size_(std::max(size_t{1}, cache_size/2)) {}

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

template <typename T, typename KeyT>
bool Cache_2q_t<T, KeyT>::in_a1(KeyT key) const {
    return a1_map_.find(key) != a1_map_.end();
}

template <typename T, typename KeyT>
bool Cache_2q_t<T, KeyT>::in_am(KeyT key) const {
    return am_map_.find(key) != am_map_.end();
}

template <typename T, typename KeyT>
bool Cache_2q_t<T, KeyT>::in_a1out(KeyT key) const {
    return a1out_map_.find(key) != a1out_map_.end();
}

template <typename T, typename KeyT>
void Cache_2q_t<T, KeyT>::insert_to_a1(const Content2Q<T, KeyT>& node) {
    if (a1_full())
    {
        KeyT victim = a1_.back().key;
        a1_map_.erase(victim);
        a1_.pop_back();
        insert_to_a1out(victim);
    }

    a1_.push_front(node);
    a1_map_.emplace(node.key, a1_.begin());
}

template <typename T, typename KeyT>
void Cache_2q_t<T, KeyT>::insert_to_am(const Content2Q<T, KeyT>& node) {
    if (am_full())
    {
        KeyT victim = am_.back().key;
        am_map_.erase(victim);
        am_.pop_back();
    }

    am_.push_front(node);
    am_map_.emplace(node.key, am_.begin());
}

template <typename T, typename KeyT>
void Cache_2q_t<T, KeyT>::insert_to_a1out(KeyT key) {
    if (a1out_full())
    {
        a1out_map_.erase(a1out_.back());
        a1out_.pop_back();
    }

    a1out_.push_front(key);
    a1out_map_.emplace(key, a1out_.begin());
}

template <typename T, typename KeyT>
template <typename F>
bool Cache_2q_t<T, KeyT>::lookup_update(KeyT key, F slow_get_page) {
    auto it_am = am_map_.find(key);
    if (it_am != am_map_.end())
    {
        am_.splice(am_.begin(), am_, it_am->second);
        return true;
    }

    auto it_a1 = a1_map_.find(key);
    if (it_a1 != a1_map_.end())
        return true;

    auto it_a1out = a1out_map_.find(key);
    if (it_a1out != a1out_map_.end())
    {
        Content2Q<T, KeyT> node = {
            .key = key,
            .page = slow_get_page(key),
        };

        a1out_.erase(it_a1out->second);
        a1out_map_.erase(it_a1out);
        insert_to_am(node);
        return false;
    }

    Content2Q<T, KeyT> node = {
        .key = key,
        .page = slow_get_page(key),
    };
    insert_to_a1(node);
    return false;
}

#endif