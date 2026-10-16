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
    page_t* page;
    int freq_cnt;
};