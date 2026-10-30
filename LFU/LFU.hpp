#pragma once

#include<list>
#include<unordered_map>

#include"../slow_get_page.hpp"

using PAGE_T = int;

template <typename cache_content, typename keyT = int> 
struct LFU_cache_t {
    size_t size;
    LFU_cache_t(size_t sz) : size(sz) {};

    std::list<cache_content> cache_;

    using ListIt = typename std::list<cache_content>::iterator;
    std::unordered_map<keyT, ListIt> hash_;

    bool is_cache_full();

    auto find_unfrequent_page();

    template <typename F> 
    bool parse_income_page(keyT key, F slow_get_page);
};

template <typename page_t>
struct LFU_content {
    int key;
    page_t page;
    int freq_cnt;
};


template <typename cache_content, typename keyT> 
bool LFU_cache_t<cache_content, keyT>::is_cache_full()
{
    if (cache_.size() < size) return false;

    return true;
}

template <typename cache_content, typename keyT> 
template <typename F>
bool LFU_cache_t<cache_content, keyT>::parse_income_page(keyT key, F slow_get_page)
{
    auto cached_page_ptr = hash_.find(key);
        
    bool cache_is_full = is_cache_full();

    if (cached_page_ptr != hash_.end()) {
        cache_.splice(cache_.begin(), cache_, cached_page_ptr->second);
        (cache_.begin()->freq_cnt)++;
        
        // process_page(key);

        return true;
    }

    auto rare_it = find_unfrequent_page();
    if (cache_is_full && rare_it != cache_.end()) {
        hash_.erase(rare_it->key);
        cache_.erase(rare_it);
    }

    LFU_content<int> newpage = {key, slow_get_page(key), 1}; 
    cache_.push_front(newpage);
    hash_.emplace(key, cache_.begin());

    // process_page(key);

    return false;
}

template <typename cache_content, typename keyT>
auto LFU_cache_t<cache_content, keyT>::find_unfrequent_page()
{
    auto it = cache_.begin();
    auto rare_it = it;
    int smallest_freq = it->freq_cnt;

    while (it != cache_.end()) {
        it++;
        
        if (it->freq_cnt <= smallest_freq) {
            smallest_freq = it->freq_cnt;
            rare_it = it;
        }
    }

    return (rare_it);
}

template bool LFU_cache_t<LFU_content<PAGE_T>, int>::parse_income_page<PAGE_T(*)(int)>(int, PAGE_T(*)(int));