#pragma once

#include <list>
#include <unordered_map>

#include "../slow_get_page.hpp"

template<typename page_t>
struct LFU_content {
    int key;
    page_t page;
    int freq_cnt;
};

template<typename cache_content, typename page_t, typename keyT = int> 
class LFU_cache_t {
private:
    size_t size;

    std::list<cache_content> cache_;

    using ListIt = typename std::list<cache_content>::iterator;
    std::unordered_map<keyT, ListIt> hash_;

public:
    LFU_cache_t (const size_t sz) : size (sz) {};

    bool is_cache_full ();
    
    auto find_unfrequent_page ();

    template<typename F, typename page_type>
    std::optional<page_t> lookup (const keyT key, const F slow_get_page);

    std::pair<keyT, page_t> insert (keyT key);

    void erase (keyT key);
};

template<typename cache_content, typename keyT> 
bool LFU_cache_t<cache_content, keyT>::is_cache_full ()
{
    if (cache_.size () < size) return false;

    return true;
}

template<typename cache_content, typename keyT> 
template<typename F, typename page_type>
std::optional<page_type> LFU_cache_t<cache_content, keyT>::lookup (const keyT key, const F slow_get_page)
{
    auto cached_page_ptr = hash_.find (key);

    bool cache_is_full = is_cache_full ();

    if (cached_page_ptr != hash_.end ()) {
        cache_.splice(cache_.begin (), cache_, cached_page_ptr->second);
        (cache_.begin ()->freq_cnt)++;

        // process_page (key);

        return cache_.front().page;
    }

    insert (key);

    return cache_.front ().page;
}

// template <typename cache_content, typename page_t, typename keyT>
// std::pair<keyT, page_t> LFU_cache_t<cache_content, keyT>::insert (keyT key)
// {
//     auto rare_it = find_unfrequent_page ();
//     if (cache_is_full && rare_it != cache_.end ()) {
//         std::pair<keyT, cache_content> evicted_page = {rare_it->key, rare_it->page};

//         erase (rare_it->key);
//     }

//     LFU_content<page_type> newpage = {key, slow_get_page (key), 1}; 
//     cache_.push_front (newpage);
//     hash_.emplace (key, cache_.begin ());

//     // process_page (key);

//     return evicted_page;
// }

template <typename cache_content, typename keyT>
void LFU_content_t<cache_content, keyT>::erase (keyT key)
{
    cache_.erase (hash_.find(key)->second);
    hash_.erase (key);
}

template <typename cache_content, typename keyT>
auto LFU_cache_t<cache_content, keyT>::find_unfrequent_page ()
{
    auto it = cache_.begin ();
    auto rare_it = it;
    int smallest_freq = it->freq_cnt;

    while (it != cache_.end ()) {
        it++;
        
        if (it->freq_cnt <= smallest_freq) {
            smallest_freq = it->freq_cnt;
            rare_it = it;
        }
    }

    return rare_it;
}