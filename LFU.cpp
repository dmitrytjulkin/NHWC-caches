#include<stdio.h>
#include<list>
#include<unordered_map>

#include"LFU.h"

void evict_unfrequent_page(std::list<LFU_content>* list, int key);

template <typename cache_content, typename keyT> 
bool cache_t<cache_content, keyT>::is_cache_full()
{
    if (cache_.size() < CACHE_SIZE) return false;

    return true;
}

template <typename cache_content, typename keyT> 
template <typename F>
bool cache_t<cache_content, keyT>::parse_income_page(keyT key, F slow_get_page) // now its only for LFU, so it shouldn't belong to cache_t
{
    cache_content* cached_page_ptr = hash_.find(key);
    bool cache_is_full = is_cache_full();

    if (cached_page_ptr != hash.end()) {
        cache_.splice(cache_.begin(), cache_, it);
        cache_.freq_cnt++;
        
        process_page(key);

        return true;
    }

    if (cache_is_full && !is_page_frequent(cache_, key)) return false;

    if (cache_is_full) evict_unfrequent_page(&cache_, key);

    LFU_content newpage = {key, slow_get_page(key), 1};
    cache_.push_front(newpage);
    hash_.emplace(key, cache_.begin());
    
    process_page(key);

    return false;
}

void evict_unfrequent_page(std::list<LFU_content>* list, int key)
{
    auto it = list->begin();
    auto rare_it = it;
    size_t smallest_freq = it->freq_cnt;

    while (it != list->end()) {
        it++;
        
        if (it->freq_cnt < smallest_freq) {
            smallest_freq = it->freq_cnt;
            rare_it = it;
        }
    }

    list->erase(rare_it);
}
