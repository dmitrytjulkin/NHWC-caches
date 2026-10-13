#include<stdio.h>
#include<iostream>
#include<print>

#include"LFU.hpp"

int main ()
{
    // 1. read_config();
    // ....

    // 2. read_input_data();
    #if 0
    std::cout << "write down size of cache, count of data, the sequence of data";

    size_t cache_sz = 0, data_cnt = 0;
    std::cin >> cache_sz >> data_cnt;

    int page_seq[INIT_SIZE] = {};
    for (int n = 0; n < data_cnt; n++)
        std::cin >> page_seq[n];
    
    int hit_cnt = 0    
    for (int it = 0; it < data_cnt; it++)
        hit_cnt += L1.parse_income_page<page_t*(*)(int)>(page_seq[it], slow_get_page);

    std::cout << GREEN << "The hit count = " << hit_cnt << COLOR_RESET << "\n";
    #endif

    // 3. run parse_income_page()

    // 4. out << hit_cnt;

    // OR run tests
    LFU_test1();

    return 0;
}