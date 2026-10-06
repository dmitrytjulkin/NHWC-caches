#pragma once

#define COLOR_RESET   "\033[0m"
#define GREEN   "\033[32m"

const int INIT_SIZE = 1000;

typedef int keyT;
typedef char page_t[1000]; // no no no no no (fr no?)

template <typename cache_content> 
struct cache_t {
    std::list<cache_content> cache_;

    // smth with the hashmap

    bool is_cache_full();
    cache_content* is_page_cached(keyT key);
    
    template <typename F> 
    bool parse_income_page(keyT key, F slow_get_page);
};

struct LFU_content {
    keyT key;
    page_t* page;
    size_t freq_cnt;
};

void slow_get_page(keyT key);
void process_page(keyT key);