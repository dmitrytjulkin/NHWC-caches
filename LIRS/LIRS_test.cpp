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

TEST(LIRS, Test1)
{
    int cache_sz = 3, data_cnt = 5;
    int cache_requests[] = {1, 2, 3, 4, 3};
 
    LIRS_cache_t<LIRS_meta<>, PAGE_T> L1(cache_sz);

    int hit_cnt = 0;
    for (int it = 0; it < data_cnt; it++) {
        hit_cnt += L1.lookup_update(cache_requests[it], slow_get_page);

        std::cout << GREEN "the try = " << it + 1 <<  COLOR_RESET ", hit count = " << hit_cnt << "\n";
        print_LIRS_data(&L1);
        print_LIRS_meta(&L1);
        std::cout << "\n";
    }

    EXPECT_EQ(hit_cnt, 0);
}

// TEST(LIRS, Test2)
// {
//     int cache_sz = 2, data_cnt = 7;
//     int cache_requests[] = {1, 1, 1, 1, 1, 1, 1};

//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 6);
// }

// TEST(LIRS, Test3)
// {
//     int cache_sz = 2, data_cnt = 7;
//     int cache_requests[] = {1, 2, 3, 4, 5, 6, 7};

//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 0);
// }

// TEST(LIRS, Test4)
// {
//     int cache_sz = 3, data_cnt = 3;
//     int cache_requests[] = {1, 2, 1};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 1);
// }

// TEST(LIRS, Test5)
// {
//     int cache_sz = 3, data_cnt = 4;
//     int cache_requests[] = {1, 2, 3, 3};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 1);
// }

// TEST(LIRS, Test6)
// {
//     int cache_sz = 3, data_cnt = 5;
//     int cache_requests[] = {1, 2, 3, 4, 3};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 0);
// }

// TEST(LIRS, Test7)
// {
//     int cache_sz = 3, data_cnt = 9;
//     int cache_requests[] = {1, 2, 3, 3, 4, 1, 4, 3, 2};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 3);
// }

