#pragma once

#include<list>
#include<unordered_map>
#include<optional>

#include"../slow_get_page.hpp"

#define GREEN "\x1B[32m"
#define COLOR_RESET "\x1B[0m"

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
    using s_list_elem_t = typename cache_list_t::iterator;

    std::optional<typename std::list<s_list_elem_t>::iterator> s_it = std::nullopt;
    std::optional<typename std::list<s_list_elem_t>::iterator> q_it = std::nullopt;
};

template <typename cache_content, typename keyT = int> 
struct LIRS_cache_t {
    size_t cache_size;
    LIRS_cache_t(size_t sz) : cache_size(sz) {};
    
    size_t HIR_size{(cache_size + 99) / 100}; // 1% of size (rounded up)
    size_t LIR_size{cache_size - HIR_size};

    size_t LIR_cnt{0};
    
    using cache_list_t = typename std::list<LIRS_content<PAGE_T, keyT>>;
    using cache_it = typename cache_list_t::iterator;

    std::list<cache_content> cache_list;
    std::list<cache_it> s_stack;
    std::list<cache_it> q_list;

    using hash_t = typename std::unordered_map<keyT, cache_it>;
    using hash_it = typename hash_t::iterator;
    hash_t cache_hash;

    template <typename F> 
    bool lookup_update(keyT key, F slow_get_page);

    void prune_stack ();

private:
    template <typename F>
    bool parse_new_page(keyT key, F slow_get_page);

    template <typename F>
    bool parse_nonresHIR_in_s(cache_content x_data, keyT key, F slow_get_page);

    template <typename F>
    bool parse_resHIR(cache_it x_data, keyT key, F slow_get_page);

    void move_s_bottom_to_q();
};

template <typename cache_content, typename keyT> 
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::lookup_update(keyT key, F slow_get_page)
{
    auto x_hash_code = cache_hash.find(key);

    if (x_hash_code == cache_hash.end())      // there was no 'x' -> miss
        return parse_new_page(key, slow_get_page);

    cache_content x_data = *x_hash_code->second;

    if (x_data.status == HIR && x_data.q_it == std::nullopt) // still a miss
        return parse_nonresHIR_in_s(x_data, key, slow_get_page);

    auto x_s_it = *x_data.s_it;

    if ((*x_s_it)->status == LIR) { // x = LIR
        s_stack.splice(s_stack.begin(), s_stack, x_s_it);
        prune_stack();

        // process_page(key);

        return true;
    }

    return parse_resHIR(x_hash_code->second, key, slow_get_page);
}

template <typename cache_content, typename keyT>
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::parse_new_page(keyT key, F slow_get_page)
{
    bool newpage_is_LIR = false;

    if (LIR_cnt < LIR_size)
        newpage_is_LIR = true;
        
    else if (q_list.size() == HIR_size) {
        cache_list.erase(*q_list.begin());
        q_list.erase(q_list.begin());           // delete old resHIR
    }

    LIRS_content<PAGE_T> newpage = {key, slow_get_page(key)};

    cache_list.push_front(newpage);         // load x page
    auto newpage_it = cache_list.begin();

    s_stack.push_front(newpage_it);
    newpage_it->s_it = s_stack.begin();

    cache_hash.emplace(key, cache_list.begin());
    
    if (newpage_is_LIR) {
        newpage_it->status = LIR;
        ++LIR_cnt;
    
    } else {
        q_list.push_back(newpage_it);           // x -> res_HIR
        newpage_it->q_it = q_list.begin();
    }

    return false;
}

template <typename cache_content, typename keyT>
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::parse_nonresHIR_in_s(cache_content x_data, keyT key, F slow_get_page)
{
    x_data.status = LIR;

    s_stack.splice(s_stack.begin(), s_stack, *(x_data.s_it));

    move_s_bottom_to_q();

    prune_stack();

    // process_page(key);

    return false;
}

template <typename cache_content, typename keyT>
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::parse_resHIR(typename LIRS_cache_t<cache_content, keyT>::cache_it x_data, keyT key, F slow_get_page)
{
    if (x_data->s_it != std::nullopt) {    // x in s
        s_stack.splice(s_stack.begin(), s_stack, *(x_data->s_it));

        x_data->status = LIR;

        q_list.erase(*(x_data->q_it));
        
        move_s_bottom_to_q();

        prune_stack();
    } else {                                        // x not in s

        s_stack.push_front(x_data);

        q_list.push_back(x_data);
    }

    // process_page(key);

    return true;
}

template <typename cache_content, typename keyT>
void LIRS_cache_t<cache_content, keyT>::move_s_bottom_to_q()
{
    auto dying_LIR = s_stack.back();

    q_list.push_back(dying_LIR);
    
    dying_LIR->status = HIR;
    
    s_stack.pop_back();  // s_bottom -> res_HIR
}

template <typename cache_content, typename keyT>
void LIRS_cache_t<cache_content, keyT>::prune_stack()
{
    while (!s_stack.empty() && s_stack.back()->status == HIR) {
        auto s_bottom_it = s_stack.back();

        s_stack.pop_back();

        if (s_bottom_it->q_it == std::nullopt) {
            cache_hash.erase(s_bottom_it->key);
            
            cache_list.erase(s_bottom_it);
        
        } else {
            s_bottom_it->s_it = std::nullopt;
        }
    }
}