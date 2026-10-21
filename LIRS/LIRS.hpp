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
    
    using list_t = typename std::list<keyT>;
    using list_it_t = typename list_t::iterator;
    
    std::optional<list_it_t> cache_it = std::nullopt;
    std::optional<list_it_t> s_it     = std::nullopt;
    std::optional<list_it_t> q_it     = std::nullopt;
};

template <typename cache_content, typename keyT = int> 
struct LIRS_cache_t {
    size_t cache_size;
    LIRS_cache_t(size_t sz) : cache_size(sz) {};
    
    size_t HIR_size{(cache_size + 99) / 100}; // 1% of size (rounded up)
    size_t LIR_size{cache_size - HIR_size};

    size_t LIR_cnt{0};
    
    using list_t = typename std::list<keyT>;
    list_t cache_list;
    list_t s_stack;
    list_t q_list;

    using hash_t = typename std::unordered_map<keyT, cache_content>;
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
    bool parse_resHIR(cache_content x_data, keyT key, F slow_get_page);

    void move_s_bottom_to_q();
};

template <typename cache_content, typename keyT> 
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::lookup_update(keyT key, F slow_get_page)
{
    auto x_hash_code = cache_hash.find(key);

    if (x_hash_code == cache_hash.end())      // there was no 'x' -> miss
        return parse_new_page(key, slow_get_page);

    cache_content x_data = x_hash_code->second;

    if (x_data.status == HIR && x_data.q_it == std::nullopt) // still a miss
        return parse_nonresHIR_in_s(x_data, key, slow_get_page);

    auto x_s_it = *x_data.s_it;

    if (x_data.status == LIR) { // x = LIR
        s_stack.splice(s_stack.begin(), s_stack, x_s_it);
        x_data.s_it = s_stack.begin();

        prune_stack();

        // process_page(key);

        return true;
    }

    return parse_resHIR(x_data, key, slow_get_page);
}

template <typename cache_content, typename keyT>
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::parse_new_page(keyT key, F slow_get_page)
{
    bool newpage_is_LIR = false;

    if (LIR_cnt < LIR_size)
        newpage_is_LIR = true;
        
    else if (q_list.size() == HIR_size) {
        auto first_q_it = q_list.begin();
        cache_list.erase(*cache_hash.find(*first_q_it)->second.cache_it);
        q_list.pop_front();           // delete old resHIR
    }

    LIRS_content<PAGE_T> newpage = {key, slow_get_page(key)};

    cache_hash.emplace(key, newpage);
    cache_content new_data = cache_hash.find(key)->second;

    cache_list.push_front(key);         // load x page
    new_data.cache_it = cache_list.begin();

    s_stack.push_front(key);
    new_data.s_it = s_stack.begin();
    
    if (newpage_is_LIR) {
        new_data.status = LIR;
        ++LIR_cnt;
    
    } else {
        q_list.push_back(key);           // x -> res_HIR
        new_data.q_it = q_list.begin();
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
bool LIRS_cache_t<cache_content, keyT>::parse_resHIR(cache_content x_data, keyT key, F slow_get_page)
{
    if (x_data.s_it != std::nullopt) {    // x in s
        s_stack.splice(s_stack.begin(), s_stack, *(x_data.s_it));
        x_data.s_it = s_stack.begin();

        x_data.status = LIR;

        q_list.erase(*(x_data.q_it));
        x_data.q_it = std::nullopt;
        
        move_s_bottom_to_q();

        prune_stack();
    } else {                                        // x not in s
        s_stack.push_front(key);
        x_data.s_it = s_stack.begin();

        q_list.push_back(key);
        x_data.q_it = q_list.end();
    }

    // process_page(key);

    return true;
}

template <typename cache_content, typename keyT>
void LIRS_cache_t<cache_content, keyT>::move_s_bottom_to_q()
{
    cache_content dying_LIR_data = cache_hash.find(s_stack.back())->second;

    q_list.push_back(dying_LIR_data.key);
    dying_LIR_data.q_it = q_list.end();
    
    dying_LIR_data.status = HIR;
    
    s_stack.pop_back();  // s_bottom -> res_HIR
    dying_LIR_data.s_it = std::nullopt;
}

template <typename cache_content, typename keyT>
void LIRS_cache_t<cache_content, keyT>::prune_stack()
{
    keyT key = s_stack.back();
    cache_content s_bottom_data = cache_hash.find(key)->second;

    while (!s_stack.empty() && s_bottom_data.status == HIR) {
        s_stack.pop_back();
        s_bottom_data.s_it = std::nullopt;

        if (s_bottom_data.q_it == std::nullopt) {
            cache_list.erase(*s_bottom_data.cache_it);

            cache_hash.erase(s_bottom_data.key); // !!! (where to store big data)
        
        }

        key = s_stack.back();
        s_bottom_data = cache_hash.find(key)->second;
    }
}