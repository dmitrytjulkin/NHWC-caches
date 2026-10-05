#include<stdio.h>
#include<list>
#include<unordered_map>

#include"LFU.h"

typedef int keyT;

template <typename cache_content> struct cache_t {
    std::list<cache_content> cache_;

    // smth with the hashmap

    bool is_cache_full();
    bool is_page_cached(keyT key);
    template <typename F> auto parse_income_page(keyT key, F slow_get_page);
};

struct LFU_content {
    keyT key;
    page_t* page;
    size_t freq_cnt;
};

template <typename cache_content> 
bool cache_t<cache_content>::is_cache_full()
{
    if (cache_.size() < CACHE_SIZE) return false;

    return true;
}

template <typename cache_content, typename F> 
bool cache_t<cache_content>::parse_income_page(keyT key, F slow_get_page) // now its only for LFU, so it shouldn't belong to cache_t
{
    if (key == 0) return false;

    if (!is_page_frequent(cache, key)) return false;

    auto page_cached = is_page_cached(cache, key);  // hashmap realisation needed

    if (is_cache_full() && page_cached == NULL) cache_.pop_back();

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

bool is_page_frequent(keyT key)
{

}

void slow_get_page(keyT key)
{
    return 0;
}

int main ()
{
    cache_t LFU;

    return 0;
}
