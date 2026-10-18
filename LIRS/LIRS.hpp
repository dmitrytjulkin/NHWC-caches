#pragma once

#include<list>
#include<unordered_map>

#include"../slow_get_page.hpp"

template <typename page_t, typename keyT = int>
struct LIRS_content {
    keyT key;
    page_t page;
};

enum page_status {
    LIR,
    res_HIR,
    non_res_HIR
};

template <typename keyT = int>
struct s_content {
    keyT key;
    page_status page;
};

template <typename cache_content, typename keyT = int> 
struct LIRS_cache_t {
    size_t cache_size;
    LFU_cache_t(size_t sz) : cache_size(sz) {};

    std::list<cache_content> cache_;

    std::list<s_content> s_stack;

    using s_it = typename std::list<s_content>::iterator;
    std::list<s_it> q_list;

    size_t q_size{(cache_size + 99) / 100}; // 1% of size (rounded up)
    size_t s_size{cache_size - q_size};

    using list_it = typename std::list<cache_content>::iterator;
    std::unordered_map<keyT, list_it> hash_;

    bool is_cache_full();

    auto find_unfrequent_page();

    template <typename F> 
    bool parse_income_page(keyT key, F slow_get_page);

    void prune_stack ();
};
