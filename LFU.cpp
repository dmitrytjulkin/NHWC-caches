#include<stdio.h>
#include<list>
#include<unordered_map>

#include"LFU.h"

void evict_unfrequent_page(std::list<LFU_content>* list, keyT key);

template <typename cache_content> 
bool cache_t<cache_content>::is_cache_full()
{
    if (cache_.size() < CACHE_SIZE) return false;

    return true;
}

template <typename cache_content> 
template <typename F>
bool cache_t<cache_content>::parse_income_page(keyT key, F slow_get_page) // now its only for LFU, so it shouldn't belong to cache_t
{
    cache_content* cached_page_ptr = is_page_cached(key);  // hashmap realisation needed
    bool cache_is_full = is_cache_full();

    if (cached_page_ptr == NULL) {
        if (cache_is_full && !is_page_frequent(cache_, key)) return false;

        if (cache_is_full) evict_unfrequent_page(&cache_, key);

        LFU_content newpage = {key, slow_get_page(key), 1}
        cache_.push_front(newpage);
        
        process_page(key);

        return dalse;
    }

    cache_.splice(cache_.begin(), cache_, it);
    cache_.freq_cnt++;
    
    process_page(key);

    return true;
}

template <typename cache_content> 
cache_content* cache_t<cache_content>::is_page_cached(keyT key)    // replace it to hashmap
{
    for (cache_content* it = cache.begin(); it != cache_end(); it++)
        if (*it.key ==  key) return it;

    return NULL;
}

void evict_unfrequent_page(std::list<LFU_content>* list, keyT key)
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

bool is_page_frequent(std::list<LFU_content>* list, keyT key)
{
    // firstly we have to find this page if it exists
    
    // otherwise: compare it to pages with frequency of 1
    auto it = list->end();

    while (it != list->begin()) {
        if (it->freq_cnt == 1)
            return true;

        it--;
    }

}
