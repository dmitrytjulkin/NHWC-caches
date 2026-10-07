#pragma once

#define COLOR_RESET   "\033[0m"
#define GREEN   "\033[32m"

const int INIT_SIZE = 1000;

typedef char page_t[INIT_SIZE]; // no no no no no (fr no?)

template <typename cache_content, typename keyT = int> 
struct cache_t {
    size_t size;
    std::list<cache_content> cache_;

    using ListIt = typename std::list<cache_content>::iterator
    std::unordered_map<keyT, ListIt> hash_;

    bool is_cache_full();
    
    template <typename F> 
    bool parse_income_page(keyT key, F slow_get_page);
};

struct LFU_content {
    int key;
    page_t* page;
    size_t freq_cnt;
};

void slow_get_page(int key);
void process_page(int key);