#include<stdio.h>
#include<list>
#include<unordered_map>

#include"LFU.h"

template <typename cache_content> struct cache_t {
    std::list<cache_content> cache_;

    // smth with the hashmap

    bool is_cache_full();

    template <typename F> bool parse_income_page(keyT key, F slow_get_page);
};

struct LFU_content {
    size_t key;
    // page_t* page;
    size_t freq_cnt;
};

template <typename cache_content> bool cache_t<cache_content>::is_cache_full()
{
    if (this->cache_.size() < CACHE_SIZE) return false;

    return true;
}

bool cache_t::parse_income_page(keyT key, F slow_get_page)
{

}

int main ()
{
    cache_t LFU;

    return 0;
}
