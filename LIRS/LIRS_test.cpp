#include<iostream>
#include"gtest/gtest.h"

#include"LIRS.hpp"
#if 0
#include"../debug.hpp"
#endif

#define COUNT_HITS                                  \
    LIRS_cache_t<LIRS_content<PAGE_T>> L1(cache_sz);\
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

