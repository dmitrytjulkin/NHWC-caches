#include "gtest/gtest.h"

#include "LIRS.hpp"
#if 0
#include "../debug.hpp"
#endif

#define COUNT_HITS                                  \
    LIRS_cache_t<LIRS_meta<>, PAGE_T> L1(cache_sz); \
                                                    \
    int hit_cnt = 0;                                \
    for (int it = 0; it < data_cnt; it++)           \
        hit_cnt += L1.lookup_update(cache_requests[it], slow_get_page);

// TEST (LIRS, Test0)
// {
//     int cache_sz = 3, data_cnt = 7;
//     int cache_requests[] = {1, 2, 3, 3, 1, 1, 1};

//     LIRS_cache_t<LIRS_meta<>, PAGE_T> L1(cache_sz);

//     int hit_cnt = 0;
//     for (int it = 0; it < data_cnt; it++) {
//         std::cout << "________________________________________________________________\n";
//         std::cout << GREEN "the try = " << it + 1 <<  COLOR_RESET ", hit count = " << hit_cnt << "\n";

//         hit_cnt += L1.lookup_update(cache_requests[it], slow_get_page);

//         print_LIRS_data(&L1);
//         print_LIRS_meta(&L1);
//         std::cout << "\n";
//     }

//     EXPECT_EQ(hit_cnt, 4);
// }

// 1. Interleaved LIR hits and HIR misses (scan resistance)
TEST (LIRS, Test1)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 1, 3, 4, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 2. Repeated hits on a single LIR page
TEST (LIRS, Test2)
{
    int cache_sz = 2, data_cnt = 7;
    int cache_requests[] = {1, 1, 1, 1, 1, 1, 1};

    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 6);
}

// 3. Sequential scan of unique pages, all misses
TEST (LIRS, Test3)
{
    int cache_sz = 2, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 4, 5, 6, 7};

    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 0);
}

// 4. Basic LIR hit after initialization
TEST (LIRS, Test4)
{
    int cache_sz = 3, data_cnt = 3;
    int cache_requests[] = {1, 2, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 1);
}

// 5. Hit on Resident HIR causing promotion to LIR
TEST (LIRS, Test5)
{
    int cache_sz = 3, data_cnt = 4;
    int cache_requests[] = {1, 2, 3, 3};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 1);
}

// 6. Miss on Non-resident HIR (page in S), causing promotion to LIR
TEST (LIRS, Test6)
{
    int cache_sz = 3, data_cnt = 5;
    int cache_requests[] = {1, 2, 3, 4, 3};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 0);
}

// 7. Complex mixed scenario with hits, promotions, and non-resident returns
TEST (LIRS, Test7)
{
    int cache_sz = 3, data_cnt = 9;
    int cache_requests[] = {1, 2, 3, 3, 4, 1, 4, 3, 2};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}

// 8. Hit on LIR page during initialization phase (LIR not full)
TEST (LIRS, Test8)
{
    int cache_sz = 3, data_cnt = 2;
    int cache_requests[] = {1, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 1);
}

// 9. Multiple hits on already promoted LIR pages
TEST (LIRS, Test9)
{
    int cache_sz = 3, data_cnt = 4;
    int cache_requests[] = {1, 2, 1, 2};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 10. Initialization, filling Q, and pure eviction (all misses)
TEST (LIRS, Test10)
{
    int cache_sz = 3, data_cnt = 4;
    int cache_requests[] = {1, 2, 3, 4};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 0);
}

// 11. Ping-pong promotion followed by non-resident resurrection
TEST (LIRS, Test11)
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 3, 1, 4, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 12. Hit on Resident HIR (not in S) keeps it HIR and moves it to Q
TEST (LIRS, Test12)
{
    int cache_sz = 3, data_cnt = 5;
    int cache_requests[] = {1, 2, 3, 3, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 13. Hit on HIR not in S, immediately followed by its eviction
TEST (LIRS, Test13)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 3, 1, 4};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 14. Hit on Resident HIR not in S, followed by eviction and promotion
TEST (LIRS, Test14)
{
    int cache_sz = 3, data_cnt = 8;
    int cache_requests[] = {1, 2, 3, 3, 1, 4, 4, 3};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 4);
}

// 15. Mixed hits on LIR and Resident HIR (status ping-pong)
TEST (LIRS, Test15)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 3, 2, 2};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}

// 16. Hot LIR page retention during a constant stream of new HIRs
TEST (LIRS, Test16)
{
    int cache_sz = 3, data_cnt = 8;
    int cache_requests[] = {1, 2, 3, 1, 4, 1, 5, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}

// 17. Miss на Non-resident HIR, которая глубоко в стеке S (проверка вытеснения Q и обрезки)
TEST (LIRS, Test17)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 4, 5, 3};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 0);
}
// 18. Miss on all pages, checking Q rotation
TEST (LIRS, Test18)
{
    int cache_sz = 3, data_cnt = 5;
    int cache_requests[] = {1, 2, 3, 4, 5};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 0);
}

// 19. Hits on initial LIR pages
TEST (LIRS, Test19)
{
    int cache_sz = 3, data_cnt = 4;
    int cache_requests[] = {1, 1, 2, 2};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 20. Consecutive promotions causing multiple HIR prunings
TEST (LIRS, Test20)
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 3, 2, 4, 5};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 21. Deep resurrection of a non-resident HIR from the bottom of S
TEST (LIRS, Test21)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 4, 5, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 1);
}

// 22. Consecutive misses on pages that were evicted from Q but kept in S
TEST (LIRS, Test22)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 4, 3, 4};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 0);
}

// 23. Frequent hits on LIR protecting it from eviction
TEST (LIRS, Test23)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 1, 4, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 24. Miss on page after it was completely evicted from cache and S
TEST (LIRS, Test24)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 4, 5, 4};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 0);
}

// 25. Mixed hits on LIR and Resident HIR
TEST (LIRS, Test25)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 1, 3, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}

// 26. HIR hit causing demotion of LIR to Q, then miss on that demoted LIR
TEST (LIRS, Test26)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 3, 1, 2};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}

// 27. Non-resident HIR miss followed immediately by a hit
TEST (LIRS, Test27)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 4, 3, 3};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 1);
}

// 28. Alternating hits between LIR and HIR pages
TEST (LIRS, Test28)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 2, 3, 2};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}

// 29. Scanning pattern: hits on LIR, misses on HIR
TEST (LIRS, Test29)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 1, 4, 2};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 30. Consecutive misses on previously evicted Non-resident HIRs
TEST (LIRS, Test30)
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 4, 3, 4, 3};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 1);
}

// 31. Complex promotion and demotion of multiple pages
TEST (LIRS, Test31)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 3, 4, 4};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 2);
}

// 32. Sequence causing demotion and subsequent promotion of different pages
TEST (LIRS, Test32)
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 3, 1, 4, 4};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}

// 33. Repeated hits on a newly promoted LIR page
TEST (LIRS, Test33)
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 3, 1, 1, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 4);
}

// 34. Miss on Non-resident HIR after it was pushed deeper in S
TEST (LIRS, Test34)
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 3, 4, 5, 3, 3};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 1);
}

// 35. Continuous hits on a single LIR page
TEST (LIRS, Test35)
{
    int cache_sz = 3, data_cnt = 5;
    int cache_requests[] = {1, 1, 1, 1, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 4);
}

// 36. Continuous hits on a single HIR page (after promotion)
TEST (LIRS, Test36)
{
    int cache_sz = 3, data_cnt = 6;
    int cache_requests[] = {1, 2, 3, 3, 3, 3};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}

// 37. Alternating LIR hits with new page misses
TEST (LIRS, Test37)
{
    int cache_sz = 3, data_cnt = 7;
    int cache_requests[] = {1, 2, 1, 3, 1, 4, 1};
    
    COUNT_HITS;

    EXPECT_EQ(hit_cnt, 3);
}
