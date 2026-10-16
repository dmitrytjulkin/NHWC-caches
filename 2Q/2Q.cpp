#include "2Q.hpp"

#include <list>
#include <unordered_map>

template <typename T, typename KeyT>
void Cache_2q_t<T, KeyT>::insert_to_a1(const Content2Q<T, KeyT>& node) {
    if (a1_full()) {
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
    if (am_full()) {
        KeyT victim = am_.back().key;
        am_map_.erase(victim);
        am_.pop_back();
    }

    am_.push_front(node);
    am_map_.emplace(node.key, am_.begin());
}

template <typename T, typename KeyT>
void Cache_2q_t<T, KeyT>::insert_to_a1out(KeyT key) {
    if (a1out_full()) {
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
    if (it_am != am_map_.end()) {
        am_.splice(am_.begin(), am_, it_am->second);
        return true;
    }

    auto it_a1 = a1_map_.find(key);
    if (it_a1 != a1_map_.end())
        return true;

    auto it_a1out = a1out_map_.find(key);
    if (it_a1out != a1out_map_.end()) {
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