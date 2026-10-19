#pragma once

#include<list>
#include<unordered_map>
#include<optional>

#include"../slow_get_page.hpp"

using PAGE_T = int;

enum page_status {
    LIR,
    HIR
};

template <typename page_t, typename keyT = int>
struct LIRS_content {
    keyT key;
    page_t page;

    page_status status = HIR;
    
    using cache_list_t = typename std::list<LIRS_content>;
    using s_list_t = typename cache_list_t::iterator;

    std::optional<std::list<s_list_t>::iterator> s_it = std::nullopt;
    std::optional<std::list<s_list_t>::iterator> q_it = std::nullopt;
};

template <typename cache_content, typename keyT = int> 
struct LIRS_cache_t {
    size_t cache_size;
    LFU_cache_t(size_t sz) : cache_size(sz) {};
    
    size_t q_size{(cache_size + 99) / 100}; // 1% of size (rounded up)
    size_t s_size{cache_size - q_size};

    LIRS_content::cache_list_t cache_list;
    LIRS_content::s_list_t s_stack;
    std::list<cache_list_t::iterator> q_list;

    using cache_it = typename cache_list_t::iterator;
    std::unordered_map<keyT, cache_it> cache_hash;

    bool is_cache_full();

    auto find_unfrequent_page();

    template <typename F> 
    bool parse_income_page(keyT key, F slow_get_page);

    void prune_stack ();

private:
    template <typename F>
    bool parse_new_page(keyT key, F slow_get_page);

    template <typename F>
    bool parse_nonresHIR_in_s(cache_it x_data, keyT key, F slow_get_page);
    
    template <typename F>
    bool parse_resHIR(cache_it x_data, keyT key, F slow_get_page);

    void move_s_bottom_to_q();
};
