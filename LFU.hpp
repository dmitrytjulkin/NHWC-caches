#pragma once

#include<list>
#include<unordered_map>

const int INIT_SIZE = 1000;

using page_t = char[INIT_SIZE];

template <typename cache_content, typename keyT = int> 
struct LFU_cache_t {
    size_t size;
    LFU_cache_t(size_t sz) : size(sz) {};

    std::list<cache_content> cache_;

    using ListIt = typename std::list<cache_content>::iterator;
    std::unordered_map<keyT, ListIt> hash_;

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
void LFU_test2();