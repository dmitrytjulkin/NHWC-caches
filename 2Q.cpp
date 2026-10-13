#include "2Q.h"

#include <list>
#include <unordered_map>
#include <unordered_set>

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
    return a1out_set_.contains(key);
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
        a1out_set_.erase(a1out_.back());
        a1out_.pop_back();
    }

    a1out_.push_front(key);
    a1out_set_.emplace(key);
}