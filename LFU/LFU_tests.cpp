#include<iostream>
#include"gtest/gtest.h"

#include"LFU.hpp"
#if 0
#include"../debug.hpp"
#endif

#define COUNT_HITS                                  \
    LFU_cache_t<LFU_content<PAGE_T>> L1(cache_sz);  \
                                                    \
    int hit_cnt = 0;                                \
    for (int it = 0; it < data_cnt; it++)           \
        hit_cnt += L1.parse_income_page<PAGE_T(*)(int)>(cache_requests[it], slow_get_page);


TEST(LFU, Test1)
{
    int cache_sz = 2, data_cnt = 7;
    int cache_requests[] = {1, 3, 1, 9, 3, 8, 3};

    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 1);
}


TEST(LFU, Test2)
{
    int cache_sz = 2, data_cnt = 7;
    int cache_requests[] = {1, 1, 1, 1, 1, 1, 1};

    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 6);
}

TEST(LFU, Test3)
{
    int cache_sz = 4, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 4, 5, 6, 7};

    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 0);
}

TEST(LFU, Test4)
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 1, 2, 4, 3, 4};

    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

TEST(LFU, Test5) 
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 1, 2, 3, 4, 1};

    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}

TEST(LFU, Test6)
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 1, 2, 3, 3};

    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 4);
}
