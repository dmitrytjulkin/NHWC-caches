#include "ARC.hpp"

#include <list>
#include <unordered_map>
#include <algorithm>

template <typename T, typename KeyT>
void Cache_arc_t<T, KeyT>::insert_to_b1(KeyT key) {
    b1_.push_front(key);
    b1_map_.emplace(key, b1_.begin());
}

template <typename T, typename KeyT>
void Cache_arc_t<T, KeyT>::insert_to_b2(KeyT key) {
    b2_.push_front(key);
    b2_map_.emplace(key, b2_.begin());
}

template <typename T, typename KeyT>
void Cache_arc_t<T, KeyT>::replace(KeyT key) {
    if ((t1_.size() > p_) || ((t1_.size() == p_) && in_b2(key))) {
        KeyT victim_key = t1_.back().key;
        t1_map_.erase(victim_key);
        t1_.pop_back();
        insert_to_b1(victim_key);
    }
    else {
        KeyT victim_key = t2_.back().key;
        t2_map_.erase(victim_key);
        t2_.pop_back();
        insert_to_b2(victim_key);
    }
}

template <typename T, typename KeyT>
void Cache_arc_t<T, KeyT>::insert_to_t1(const ContentARC<T, KeyT>& node) {
    t1_.push_front(node);
    t1_map_.emplace(node.key, t1_.begin());
}

template <typename T, typename KeyT>
void Cache_arc_t<T, KeyT>::insert_to_t2(const ContentARC<T, KeyT>& node) {
    t2_.push_front(node);
    t2_map_.emplace(node.key, t2_.begin());
}

template <typename T, typename KeyT>
template <typename F>
bool Cache_arc_t<T, KeyT>::lookup_update(KeyT key, F slow_get_page) {
    if (in_t1(key)) {
        auto it = t1_map_.find(key);
        t2_.splice(t2_.begin(), t1_, it->second);
        t1_map_.erase(key);
        t2_map_.emplace(key, t2_.begin());
        return true;
    }

    else if (in_t2(key)) {
        auto it = t2_map_.find(key);
        t2_.splice(t2_.begin(), t2_, it->second);
        return true;
    }

    else if (in_b1(key)) {
        if (b2_.size() < b1_.size())
            p_++;
        else
            p_ += (b2_.size()/b1_.size());

        p_ = std::min(p_, cache_size_);
    }

    return false;
}