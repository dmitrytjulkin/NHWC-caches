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
    auto cached_x = cache_hash.find(key);

    if (cached_x == cache_hash.end())      // there was no 'x' -> miss
        return parse_new_page(cached_x, key, slow_get_page);

    if (cached_x->second.status = HIR && cached_x->second.q_it = std::nullopt) // still a miss
        return parse_nonresHIR_in_s(cached_x, key, slow_get_page);

    auto x_s_ptr = cached_x->second.s_it;

    if (x_s_ptr->second.status == LIR) { // x = LIR
        s_stack.splice(s_stack.begin(), s_stack, x_s_ptr);
        prune_stack();

        // process_page(key);

        return true;
    }

    return parse_resHIR(cached_x, key, slow_get_page);
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
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::parse_nonresHIR_in_s(hash_it cached_x, keyT key, F slow_get_page)
{
    cached_x->second.status = LIR;

    s_stack.splice(s_stack.begin(), s_stack, cached_x->second.s_it);

    auto dying_LIR = *s_stack.back();
    q_list.push_back(dying_LIR);
    dying_LIR->status = HIR;
    s_stack.erase(s_stack.back());  // s_bottom -> res_HIR

    prune_stack();

    // process_page(key);

    return false;
}

template <typename cache_content, typename keyT>
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::parse_resHIR(hash_it cached_x, keyT key, F slow_get_page)
{
    if (cached_x->second.s_it != std::nullopt) { // x in s
        s_stack.splice(s_stack.begin(), s_stack, x_s_ptr);

        cached_x->second.status = LIR;

        q_list.erase(cached_x->second.q_it);
        
        auto dying_LIR = *s_stack.back();
        q_list.push_back(dying_LIR);
        dying_LIR->status = HIR;
        s_stack.erase(s_stack.back());  // s_bottom -> res_HIR

        prune_stack();
    } else { // x not in s
        s_stack.push_front(cached_x->second);

        q_list.push_back(cached_x->second);
    }

    return true;
}

// template <typename cache_content, typename keyT>
// auto LFU_cache_t<cache_content, keyT>::find_unfrequent_page()
// {
//     auto it = cache_.begin();
//     auto rare_it = it;
//     int smallest_freq = it->freq_cnt;

//     while (it != cache_.end()) {
//         it++;
        
//         if (it->freq_cnt <= smallest_freq) {
//             smallest_freq = it->freq_cnt;
//             rare_it = it;
//         }
//     }

//     return (rare_it);
// }

template bool LIRS_cache_t<LIRS_content<PAGE_T>, int>::parse_income_page<PAGE_T(*)(int)>(int, PAGE_T(*)(int));
template bool LIRS_cache_t<LIRS_content<PAGE_T>, int>::parse_new_page<PAGE_T(*)(int)>(int, PAGE_T(*)(int));
