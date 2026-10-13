#pragma once

#include<list>
#include<unordered_map>

#define COLOR_RESET   "\033[0m"
#define GREEN   "\033[32m"

const int INIT_SIZE = 1000;

typedef char page_t[INIT_SIZE]; // no no no no no (fr no?)

template <typename cache_content, typename keyT = int> 
struct cache_t {
    size_t size;
    std::list<cache_content> cache_;

    using ListIt = typename std::list<cache_content>::iterator;
    std::unordered_map<keyT, ListIt> hash_;

    cache_t(size_t sz) : size(sz) {};

    bool is_cache_full();
    void evict_unfrequent_page();

    template <typename F> 
    bool parse_income_page(keyT key, F slow_get_page);
};

struct LFU_content {
    int key;
    page_t* page;
    int freq_cnt;
};

page_t* slow_get_page(int key);
void process_page(int key);

void LFU_test1();