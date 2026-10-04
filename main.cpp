#include<stdio.h>
#include<list>
#include<unordered_map>

#include"LFU.h"

struct cache_t {
    std::list<int> cache_;

    // smth with the hashmap

    bool is_cache_full();

    template <typename F> bool parse_income_page(keyT key, F slow_get_page);
};

bool cache_t::is_cache_full()
{

}

bool cache_t::parse_income_page(keyT key, F slow_get_page)
{

}

int main ()
{
    cache_t LFU;

    return 0;
}
