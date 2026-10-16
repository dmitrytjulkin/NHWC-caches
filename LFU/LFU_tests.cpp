#include<iostream>
#include"gtest/gtest.h"

#include"LFU.hpp"
#include"../debug.hpp"

TEST(LFU, Test1)
{
    int cache_sz = 2, data_cnt = 7;
    int cache_requests[] = {1, 2, 1, 3, 2, 5, 2};

    LFU_cache_t<LFU_content<PAGE_T>> L1(cache_sz);

    int hit_cnt = 0;
    for (int it = 0; it < data_cnt; it++)
        hit_cnt += L1.parse_income_page<PAGE_T(*)(int)>(cache_requests[it], slow_get_page);

    EXPECT_EQ(hit_cnt, 1);
    std::cout << "The hit count = " << hit_cnt << "\n";
}

TEST(LFU, Test2)
{
    int cache_sz = 2, data_cnt = 7;
    int cache_requests[] = {1, 1, 1, 1, 1, 1, 1};

    LFU_cache_t<LFU_content<PAGE_T>> L1(cache_sz);

    int hit_cnt = 0;
    for (int it = 0; it < data_cnt; it++)
        hit_cnt += L1.parse_income_page<PAGE_T(*)(int)>(cache_requests[it], slow_get_page);

    EXPECT_EQ(hit_cnt, 6);
    std::cout << "The hit count = " << hit_cnt << "\n";
}

TEST(LFU, Test3)
{
    int cache_sz = 2, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 4, 5, 6, 7};

    LFU_cache_t<LFU_content<PAGE_T>> L1(cache_sz);

    int hit_cnt = 0;
    for (int it = 0; it < data_cnt; it++)
        hit_cnt += L1.parse_income_page<PAGE_T(*)(int)>(cache_requests[it], slow_get_page);

    EXPECT_EQ(hit_cnt, 0);
    std::cout << "The hit count = " << hit_cnt << "\n";
}