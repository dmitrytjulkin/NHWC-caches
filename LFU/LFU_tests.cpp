#include<iostream>
#include"gtest/gtest.h"

#include"LFU.hpp"
#include"../debug.hpp"

TEST(LFU, Test1)
{
    int cache_sz = 2, data_cnt = 7;
    int cache_requests[] = {1, 2, 1, 3, 2, 5, 2};

    LFU_cache_t<LFU_content> L1(cache_sz);

    int hit_cnt = 0;
    for (int it = 0; it < data_cnt; it++)
        hit_cnt += L1.parse_income_page<page_t*(*)(int)>(cache_requests[it], slow_get_page);

    // The hit count = 1
    std::cout << GREEN << "The hit count = " << hit_cnt << COLOR_RESET << "\n";
}

// TEST(LFU, Test2)
// {
//     int cache_sz = 2, data_cnt = 7;
//     int cache_requests[] = {1, 1, 1, 1, 1, 1, 1};

//     LFU_cache_t<LFU_content> L1(cache_sz);

//     int hit_cnt = 0;
//     for (int it = 0; it < data_cnt; it++)
//         hit_cnt += L1.parse_income_page<page_t*(*)(int)>(cache_requests[it], slow_get_page);

//     // The hit count = 6
//     std::cout << GREEN << "The hit count = " << hit_cnt << COLOR_RESET << "\n";
// }