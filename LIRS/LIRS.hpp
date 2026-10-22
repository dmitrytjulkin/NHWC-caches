#pragma once

#include<list>
#include<unordered_map>
#include<optional>
#include<assert.h>

#include"../slow_get_page.hpp"

#define GREEN "\x1B[32m"
#define COLOR_RESET "\x1B[0m"

using PAGE_T = int;

enum page_status {
    LIR,
    HIR
};

template <typename keyT = int>
struct LIRS_meta {
    keyT key;

    page_status status = HIR;
    
    using list_it_t = typename std::list<keyT>::iterator;
    
    std::optional<list_it_t> cache_it = std::nullopt;
    std::optional<list_it_t> s_it     = std::nullopt;
    std::optional<list_it_t> q_it     = std::nullopt;
};

template <typename meta, typename page_t, typename keyT = int> 
struct LIRS_cache_t {
    size_t cache_size;
    LIRS_cache_t(size_t sz) : cache_size(sz) {};
    
    size_t HIR_size{(cache_size + 99) / 100}; // 1% of size (rounded up)
    size_t LIR_size{cache_size - HIR_size};

    size_t LIR_cnt{0};
    
    std::list<page_t> cache_list;
    
    std::list<keyT> s_stack;
    std::list<keyT> q_list;

    std::unordered_map<keyT, meta> cache_hash;

    template <typename F> 
    bool lookup_update(keyT key, F slow_get_page);

    void prune_stack ();

private:
    template <typename F>
    bool parse_new_page(keyT key, F slow_get_page);

    template <typename F>
    bool parse_nonresHIR_in_s(meta* x_meta, keyT key, F slow_get_page);

    template <typename F>
    bool parse_resHIR(meta* x_meta, keyT key, F slow_get_page);

    void delete_front_q ();
    void move_s_bottom_to_q();
};

template <typename meta, typename page_t, typename keyT> 
template <typename F>
bool LIRS_cache_t<meta, page_t, keyT>::lookup_update(keyT key, F slow_get_page)
{
    auto x_hash_code = cache_hash.find(key);
    
    if (x_hash_code == cache_hash.end())        // there was no 'x' -> miss
        return parse_new_page(key, slow_get_page);

    meta& x_meta = x_hash_code->second;

    if (x_meta.status == HIR && x_meta.q_it == std::nullopt) // still a miss
        return parse_nonresHIR_in_s(&x_meta, key, slow_get_page);

    auto& x_s_it = *x_meta.s_it;

    if (x_meta.status == LIR) { // x = LIR
        s_stack.splice(s_stack.begin(), s_stack, x_s_it);
        x_meta.s_it = s_stack.begin();

        prune_stack();

        // process_page(key);

        return true;
    }

    return parse_resHIR(&x_meta, key, slow_get_page);
}

template <typename meta, typename page_t, typename keyT> 
template <typename F>
bool LIRS_cache_t<meta, page_t, keyT>::parse_new_page(keyT key, F slow_get_page)
{
    bool newpage_is_LIR = false;

    if (LIR_cnt < LIR_size)
        newpage_is_LIR = true;
        
    else if (q_list.size() == HIR_size)
        delete_front_q();

    cache_hash.emplace(key, LIRS_meta<>{.key = key});
    meta& new_meta = cache_hash.find(key)->second;

    cache_list.push_front(slow_get_page(key));  // load x page
    new_meta.cache_it = cache_list.begin();

    s_stack.push_front(key);
    new_meta.s_it = s_stack.begin();
    
    if (newpage_is_LIR) {
        new_meta.status = LIR;
        ++LIR_cnt;
    
    } else {
        q_list.push_back(key);                  // x -> res_HIR
        new_meta.q_it = q_list.begin();
    }

    return false;
}

template <typename meta, typename page_t, typename keyT> 
template <typename F>
bool LIRS_cache_t<meta, page_t, keyT>::parse_nonresHIR_in_s(meta* x_meta, keyT key, F slow_get_page)
{
    delete_front_q();

    cache_list.push_front(slow_get_page(key));
    x_meta->cache_it = cache_list.begin();
   
    x_meta->status = LIR;

    s_stack.splice(s_stack.begin(), s_stack, *(x_meta->s_it));
    x_meta->s_it = s_stack.begin();

    move_s_bottom_to_q();

    prune_stack();

    // process_page(key);

    return false;
}

template <typename meta, typename page_t, typename keyT> 
void LIRS_cache_t<meta, page_t, keyT>::delete_front_q ()
{
    auto evicted_key = *q_list.begin();
    meta& evicted_page = cache_hash.find(evicted_key)->second;

    cache_list.erase(*evicted_page.cache_it);
    q_list.pop_front();                     // delete old resHIR

    if (evicted_page.s_it == std::nullopt) cache_hash.erase(evicted_key);

    else {
        evicted_page.cache_it = std::nullopt;
        evicted_page.q_it = std::nullopt;
    }
}

template <typename meta, typename page_t, typename keyT> 
template <typename F>
bool LIRS_cache_t<meta, page_t, keyT>::parse_resHIR(meta* x_meta, keyT key, F slow_get_page)
{
    if (x_meta->s_it != std::nullopt) {    // x in s
        s_stack.splice(s_stack.begin(), s_stack, *x_meta->s_it);
        x_meta->s_it = s_stack.begin();

        x_meta->status = LIR;
            
        q_list.erase(*(x_meta->q_it));

        x_meta->q_it = std::nullopt;
        
        move_s_bottom_to_q();

        prune_stack();
    } else {                                        // x not in s
        s_stack.push_front(key);
        x_meta->s_it = s_stack.begin();

        q_list.splice(q_list.end(), q_list, *x_meta->q_it);
        x_meta->q_it = --q_list.end();
    }

    // process_page(key);

    return true;
}

template <typename meta, typename page_t, typename keyT> 
void LIRS_cache_t<meta, page_t, keyT>::move_s_bottom_to_q()
{
    meta& dying_LIR_data = cache_hash.find(s_stack.back())->second;

    q_list.push_back(dying_LIR_data.key);
    dying_LIR_data.q_it = --q_list.end();
    
    dying_LIR_data.status = HIR;
    
    s_stack.pop_back();  // s_bottom -> res_HIR
    dying_LIR_data.s_it = std::nullopt;
}

template <typename meta, typename page_t, typename keyT> 
void LIRS_cache_t<meta, page_t, keyT>::prune_stack()
{
    keyT key = s_stack.back();
    meta& s_bottom_data = cache_hash.find(key)->second;

    while (!s_stack.empty() && s_bottom_data.status == HIR) {
        s_stack.pop_back();
        s_bottom_data.s_it = std::nullopt;

        if (s_bottom_data.q_it == std::nullopt)
            cache_hash.erase(s_bottom_data.key);

        key = s_stack.back();
        s_bottom_data = cache_hash.find(key)->second;
    }
}