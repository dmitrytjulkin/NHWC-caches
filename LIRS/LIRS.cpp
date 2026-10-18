#include<iostream>

#include"LIRS.hpp"
#if 0
#include"../debug.hpp"
#endif

// template <typename cache_content, typename keyT> 
// bool LFU_cache_t<cache_content, keyT>::is_cache_full()
// {
//     if (cache_.size() < size) return false;

//     return true;
// }

template <typename cache_content, typename keyT> 
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::parse_income_page(keyT key, F slow_get_page)
{
    auto x_cache_ptr = cache_hash.find(key);
        
    // bool cache_is_full = is_cache_full();

    if (x_cache_ptr == cache_hash.end()) {      // there was no 'x' -> miss
        parse_new_page(key, slow_get_page);

        // cache_list.erase(*q_list.begin();); 
        // q_list.erase(q_list.begin());           // delete old resHIR

        // LIRS_content<PAGE_T> newpage = {key, slow_get_page(key)};
        // cache_list.push_front(newpage);         // load x page
        // s_stack.push_front(cache_list.begin());
        // q_list.push_back(cache_list.begin());   // x -> res_HIR

        // return false
    }

    if (x_cache_ptr->second.status = HIR && x_cache_ptr->second.q_it = std::nullopt) { // still a miss
        x_cache_ptr->second.status = LIR;

        s_stack.splice(s_stack.begin(), s_stack, x_cache_ptr->second.s_it);

        auto dying_LIR = *s_stack.back();
        q_list.push_back(dying_LIR);
        dying_LIR->status = HIR;
        s_stack.erase(s_stack.back());  // s_bottom -> res_HIR

        prune_stack();

        // process_page(key);

        return false;
    }

    auto x_s_ptr = x_cache_ptr->second.s_it;

    if (x_s_ptr != s_hash.end() && x_s_ptr->second.status == LIR) { // x = LIR
        s_stack.splice(s_stack.begin(), s_stack, x_s_ptr->second);
        prune_stack();

        return true;
    }

    // x = HIR

    if (x_s_ptr != s_hash.end()) {
        x_s_ptr->second.status = LIR;


    }

    s_stack.splice(s_stack.begin(), s_stack, x_s_ptr->second);


    auto rare_it = find_unfrequent_page();
    if (cache_is_full && rare_it != cache_.end()) {
        hash_.erase(rare_it->key);
        cache_.erase(rare_it);
    }

    LFU_content<int> newpage = {key, slow_get_page(key)}; 
    cache_.push_front(newpage);
    hash_.emplace(key, cache_.begin());

    // process_page(key);

    return true;
}

template <typename cache_content, typename keyT>
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::parse_new_page(keyT key, F slow_get_page)
{
    cache_list.erase(*q_list.begin();); 
    q_list.erase(q_list.begin());           // delete old resHIR

    LIRS_content<PAGE_T> newpage = {key, slow_get_page(key)};
    cache_list.push_front(newpage);         // load x page
    s_stack.push_front(cache_list.begin());
    q_list.push_back(cache_list.begin());   // x -> res_HIR

    return false
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

template bool LIRS_cache_t<LIRS_content<PAGE_T>, int>::parse_income_page<PAGE_T(*)(int)>(int, PAGE_T(*)(int));
template bool LIRS_cache_t<LIRS_content<PAGE_T>, int>::parse_new_page<PAGE_T(*)(int)>(int, PAGE_T(*)(int));
