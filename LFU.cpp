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
    if (key == 0) return false; // key == 0 - bad checking

    if (is_cache_full() && !is_page_frequent(cache, key)) return false;

    auto page_cached = is_page_cached(cache, key);  // hashmap realisation needed

    if (is_cache_full() && page_cached == NULL) evict_unfrequent_page(&cache_, key);

    if (page_cached == NULL) {
        LFU_content newpage = {key, slow_get_page(key), 1}
        cache_.push_front(newpage);
    }
    else {
        cache_.splice(cache_.begin(), cache_, it);
        cache_.freq_cnt++;
    }

    process_page(key);

    return true;
}

template <typename cache_content> 
auto cache_t<cache_content>::is_page_cached(keyT key)    // replace it to hashmap
{
    auto it = cache_.begin();

    for (auto it = cache.begin(); it != cache_end(); it++)
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
