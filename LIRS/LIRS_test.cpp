#include<iostream>
#include"gtest/gtest.h"

#include"LIRS.hpp"
#if 1
#include"../debug.hpp"
#endif

#define COUNT_HITS                                  \
    LIRS_cache_t<LIRS_content<PAGE_T>> L1(cache_sz);\
                                                    \
    int hit_cnt = 0;                                \
    for (int it = 0; it < data_cnt; it++)           \
        hit_cnt += L1.lookup_update(cache_requests[it], slow_get_page);

TEST(LFU, Test1)
{
    int cache_sz = 2, data_cnt = 7;
    int cache_requests[] = {1, 1, 1, 1, 1, 1, 1};

    LIRS_cache_t<LIRS_content<PAGE_T>> L1(cache_sz);

    int hit_cnt = 0;
    for (int it = 0; it < data_cnt; it++) {
        hit_cnt += L1.lookup_update(cache_requests[it], slow_get_page);
        
        std::cout << GREEN "the try = " << it <<  COLOR_RESET ", hit count = " << hit_cnt << "\n";
        print_LIRS_data(&L1);
        print_LIRS_meta(&L1);
        std::cout << "\n";
    }

    EXPECT_EQ(hit_cnt, 6);
}

