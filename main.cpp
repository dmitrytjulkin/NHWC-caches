#include<stdio.h>
#include<iostream>
#include<print>

#include"LFU.hpp"

int main ()
{
    // read_config();
    // ....

    // read_input_data();
    #if 0
    std::cout << "write down size of cache, count of data, the sequence of data";

    size_t cache_sz = 0, data_cnt = 0;
    std::cin >> cache_sz >> data_cnt;

    int page_seq[INIT_SIZE] = {};
    for (int n = 0; n < data_cnt; n++)
        std::cin >> page_seq[n];
    #endif

    #if 1
    int cache_sz = 2, data_cnt = 0;
    int cache_requests[] = {1, 2, 1, 3, 2, 5, 2};
    #endif

    // go through LFU algorithm
    cache_t<LFU_content> L1 = {};

    int hit_cnt = 0;
    for (int it = 0; it < data_cnt; it++) {
        hit_cnt += L1.parse_income_page<void(*)(int)>(it, slow_get_page);
    }

    // print result
    std::cout << GREEN << "The hit count = " << hit_cnt << COLOR_RESET << "\n";

    return 0;
}

