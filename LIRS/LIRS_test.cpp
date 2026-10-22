#include<iostream>
#include"gtest/gtest.h"

#include"LIRS.hpp"
#if 1
#include"../debug.hpp"
#endif

#define COUNT_HITS                                  \
    LIRS_cache_t<LIRS_meta<>, PAGE_T> L1(cache_sz); \
                                                    \
    int hit_cnt = 0;                                \
    for (int it = 0; it < data_cnt; it++)           \
        hit_cnt += L1.lookup_update(cache_requests[it], slow_get_page);

TEST(LIRS, Test1)
{
    int cache_sz = 3, data_cnt = 9;
    int cache_requests[] = {1, 2, 3, 3, 4, 1, 4, 3, 2};

    LIRS_cache_t<LIRS_meta<>, PAGE_T> L1(cache_sz);

    int hit_cnt = 0;
    for (int it = 0; it < data_cnt; it++) {
        hit_cnt += L1.lookup_update(cache_requests[it], slow_get_page);

        std::cout << GREEN "the try = " << it + 1 <<  COLOR_RESET ", hit count = " << hit_cnt << "\n";
        print_LIRS_data(&L1);
        print_LIRS_meta(&L1);
        std::cout << "\n";
    }

    EXPECT_EQ(hit_cnt, 3);
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

// // 8. Hit на LIR страницу на этапе инициализации (LIR еще не заполнен)
// TEST(LIRS, Test8)
// {
//     int cache_sz = 3, data_cnt = 2;
//     int cache_requests[] = {1, 1};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 1);
// }

// // 9. Множественные Hit на LIR страницы (когда LIR уже заполнен)
// TEST(LIRS, Test9)
// {
//     int cache_sz = 3, data_cnt = 4;
//     int cache_requests[] = {1, 2, 1, 2};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 2);
// }

// // 10. Инициализация, заполнение Q и вытеснение из Q (все промахи)
// TEST(LIRS, Test10)
// {
//     int cache_sz = 3, data_cnt = 4;
//     int cache_requests[] = {1, 2, 3, 4};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 0);
// }

// // 11. Hit на Resident HIR (страница есть в S): повышение до LIR, понижение нижнего LIR
// TEST(LIRS, Test11)
// {
//     int cache_sz = 3, data_cnt = 4;
//     int cache_requests[] = {1, 2, 3, 3};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 1);
// }

// // 12. Hit на Resident HIR (страницы НЕТ в S): она остается HIR, идет в конец Q
// TEST(LIRS, Test12)
// {
//     int cache_sz = 3, data_cnt = 5;
//     int cache_requests[] = {1, 2, 3, 3, 1};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 2);
// }

// // 13. Miss на Non-resident HIR (страница в S): повышение до LIR, вытеснение из Q
// TEST(LIRS, Test13)
// {
//     int cache_sz = 3, data_cnt = 5;
//     int cache_requests[] = {1, 2, 3, 4, 3};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 0);
// }

// // 14. Miss на "забытую" страницу (была non-resident, но pruning удалил её из S)
// // Воспринимается как абсолютно новая страница, идет в HIR
// TEST(LIRS, Test14)
// {
//     int cache_sz = 3, data_cnt = 8;
//     int cache_requests[] = {1, 2, 3, 3, 1, 4, 4, 3};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 3);
// }

// // 15. Смешанные хиты на LIR и Resident HIR (проверка пинг-понга статусов)
// TEST(LIRS, Test15)
// {
//     int cache_sz = 3, data_cnt = 6;
//     int cache_requests[] = {1, 2, 3, 3, 2, 2};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 3);
// }

// // 16. Удержание "горячей" LIR страницы при постоянном потоке новых HIR
// TEST(LIRS, Test16)
// {
//     int cache_sz = 3, data_cnt = 8;
//     int cache_requests[] = {1, 2, 3, 1, 4, 1, 5, 1};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 3);
// }

// // 17. Miss на Non-resident HIR, которая глубоко в стеке S (проверка вытеснения Q и обрезки)
// TEST(LIRS, Test17)
// {
//     int cache_sz = 3, data_cnt = 6;
//     int cache_requests[] = {1, 2, 3, 4, 5, 3};
    
//     COUNT_HITS;

//     EXPECT_EQ(hit_cnt, 0);
// }