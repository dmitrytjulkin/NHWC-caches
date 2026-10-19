#include<iostream>

#include"LIRS.hpp"
#if 0
#include"../debug.hpp"
#endif

template <typename cache_content, typename keyT> 
template <typename F>
bool LIRS_cache_t<cache_content, keyT>::parse_income_page(keyT key, F slow_get_page)
{
    auto cached_x = cache_hash.find(key);

    if (cached_x == cache_hash.end())      // there was no 'x' -> miss
        return parse_new_page(cached_x, key, slow_get_page);

    auto x_data = x_data;

    if (x_data.status = HIR && x_data.q_it = std::nullopt) // still a miss
        return parse_nonresHIR_in_s(x_data, key, slow_get_page);

    auto x_s_ptr = x_data.s_it;

    if (x_s_ptr->second.status == LIR) { // x = LIR
        s_stack.splice(s_stack.begin(), s_stack, x_s_ptr);
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
bool LIRS_cache_t<cache_content, keyT>::parse_nonresHIR_in_s(cache_it x_data, keyT key, F slow_get_page)
{
    x_data.status = LIR;

    s_stack.splice(s_stack.begin(), s_stack, x_data.s_it);

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
bool LIRS_cache_t<cache_content, keyT>::parse_resHIR(cache_it x_data, keyT key, F slow_get_page)
{
    if (x_data.s_it != std::nullopt) {    // x in s
        s_stack.splice(s_stack.begin(), s_stack, x_s_ptr);

        x_data.status = LIR;

        q_list.erase(x_data.q_it);
        
        auto dying_LIR = *s_stack.back();
        q_list.push_back(dying_LIR);
        dying_LIR->status = HIR;
        s_stack.erase(s_stack.back());  // s_bottom -> res_HIR

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
    auto dying_LIR = *s_stack.back();

    q_list.push_back(dying_LIR);
    
    dying_LIR->status = HIR;
    
    s_stack.erase(s_stack.back());  // s_bottom -> res_HIR
}

template <typename cache_content, typename keyT>
void LIRS_cache_t<cache_content, keyT>::prune_stack()
{
    while (!s_stack.empty() && s_stack.back()->status == HIR) {
        auto s_bottom_it = s_stack.back();

        s_stack.pop_back();

        if (s_bottom_it->q_list == std::nullopt) {
            cache_hash.erase(s_bottom_it->key);
            
            cache_list.erase(*s_bottom_it);
        
        } else {
            s_bottom_it->s_it = std::nullopt;
        }

    }
}

template bool LIRS_cache_t<LIRS_content<PAGE_T>, int>::parse_income_page<PAGE_T(*)(int)>(int, PAGE_T(*)(int));
template bool LIRS_cache_t<LIRS_content<PAGE_T>, int>::parse_new_page<PAGE_T(*)(int)>(int, PAGE_T(*)(int));
template bool LIRS_cache_t<LIRS_content<PAGE_T>, int>::parse_nonresHIR_in_s<PAGE_T(*)(int)>(cache_it, int, PAGE_T(*)(int));
template bool LIRS_cache_t<LIRS_content<PAGE_T>, int>::parse_resHIR<PAGE_T(*)(int)>(cache_it, int, PAGE_T(*)(int));