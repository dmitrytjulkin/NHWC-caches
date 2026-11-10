#include <gtest/gtest.h>

#include "2Q.hpp"
#include "../slow_get_page.hpp"

TEST(Cache2QBasic, MissOnEmpty) {
    Cache_2q_t<int> cache(4);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
}

TEST(Cache2QBasic, FirstMissThenHit) {
    Cache_2q_t<int> cache(4);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_TRUE(cache.lookup_update(1, slow_get_page));
}

TEST(Cache2QBasic, DifferentKeysAllMiss) {
    Cache_2q_t<int> cache(4);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(2, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(3, slow_get_page));
}

TEST(Cache2QBasic, RepeatAfterLongSequence) {
    Cache_2q_t<int> cache(4);
    for (int i = 1; i <= 10; i++)
        cache.lookup_update(i, slow_get_page);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
}

TEST(Cache2QState, FirstInsertGoesToA1) {
    Cache_2q_t<int> cache(8);
    cache.lookup_update(42, slow_get_page);
    EXPECT_TRUE(cache.in_a1(42));
    EXPECT_FALSE(cache.in_am(42));
    EXPECT_FALSE(cache.in_a1out(42));
}

TEST(Cache2QState, HitInA1DoesNotPromote) {
    Cache_2q_t<int> cache(8);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    
    EXPECT_TRUE(cache.in_a1(1));
    EXPECT_FALSE(cache.in_am(1));
}

TEST(Cache2QState, GhostPromotesToAm) {
    Cache_2q_t<int> cache(2);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);

    EXPECT_TRUE(cache.in_a1out(1));

    cache.lookup_update(1, slow_get_page);

    EXPECT_FALSE(cache.in_a1out(1));
    EXPECT_TRUE(cache.in_am(1));
    EXPECT_FALSE(cache.in_a1(1));
}

TEST(Cache2QState, HitInAmStaysInAm) {
    Cache_2q_t<int> cache(2);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(1, slow_get_page);

    cache.lookup_update(1, slow_get_page);
    EXPECT_TRUE(cache.in_am(1));
    EXPECT_FALSE(cache.in_a1(1));
}

TEST(Cache2QEviction, A1OverflowGoesToGhost) {
    Cache_2q_t<int> cache(4);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);

    EXPECT_TRUE(cache.in_a1out(1));
    EXPECT_FALSE(cache.in_a1(1));
    EXPECT_TRUE(cache.in_a1(2));
}

TEST(Cache2QEviction, AmOverflowDoesNotGoToGhost) {
    Cache_2q_t<int> cache(4);

    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(99, slow_get_page);
    cache.lookup_update(1, slow_get_page);

    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(98, slow_get_page);
    cache.lookup_update(2, slow_get_page);

    cache.lookup_update(3, slow_get_page);
    cache.lookup_update(97, slow_get_page);
    cache.lookup_update(3, slow_get_page);

    cache.lookup_update(4, slow_get_page);
    cache.lookup_update(96, slow_get_page);
    cache.lookup_update(4, slow_get_page);

    EXPECT_FALSE(cache.in_am(1));
    EXPECT_FALSE(cache.in_a1out(1));
    EXPECT_FALSE(cache.in_a1(1));
}

TEST(Cache2QEviction, GhostOverflowRemovesOldest) {
    Cache_2q_t<int> cache(4);

    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(3, slow_get_page);
    cache.lookup_update(4, slow_get_page);

    EXPECT_FALSE(cache.in_a1out(1));
    EXPECT_TRUE(cache.in_a1out(2));
    EXPECT_TRUE(cache.in_a1out(3));
}

TEST(Cache2QBoundary, CacheSizeOne) {
    Cache_2q_t<int> cache(1);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(2, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
}

TEST(Cache2QBoundary, CacheSizeTwo) {
    Cache_2q_t<int> cache(2);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(2, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_TRUE(cache.lookup_update(1, slow_get_page));
}

TEST(Cache2QPattern, HotKeySurvives) {
    Cache_2q_t<int> cache(16);

    cache.lookup_update(1, slow_get_page);
    for (int i = 100; i < 110; i++)
        cache.lookup_update(i, slow_get_page);
    cache.lookup_update(1, slow_get_page);

    EXPECT_TRUE(cache.in_am(1));

    for (int i = 200; i < 230; i++)
        cache.lookup_update(i, slow_get_page);

    EXPECT_TRUE(cache.in_am(1));
    EXPECT_TRUE(cache.lookup_update(1, slow_get_page));
}

TEST(Cache2QPattern, SequentialScanAllMiss) {
    Cache_2q_t<int> cache(4);
    for (int i = 1; i <= 20; i++) {
        EXPECT_FALSE(cache.lookup_update(i, slow_get_page)) << "i = " << i;
    }
}

TEST(Cache2QPattern, SameKeyRepeated) {
    Cache_2q_t<int> cache(4);
    EXPECT_FALSE(cache.lookup_update(7, slow_get_page));
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(cache.lookup_update(7, slow_get_page)) << "iteration " << i;
    }
}

TEST(Cache2QInvariant, KeyNotInBothA1AndAm) {
    Cache_2q_t<int> cache(8);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(3, slow_get_page);

    for (int k = 1; k <= 3; k++) {
        bool in_a1 = cache.in_a1(k);
        bool in_am = cache.in_am(k);
        EXPECT_FALSE(in_a1 && in_am) << "key " << k;
    }
}

TEST(Cache2QInvariant, KeyNotInBothAmAndGhost) {
    Cache_2q_t<int> cache(4);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(1, slow_get_page);

    EXPECT_FALSE(cache.in_am(1) && cache.in_a1out(1));
}

TEST(Cache2QInvariant, KeyNotInBothA1AndGhost) {
    Cache_2q_t<int> cache(4);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);

    EXPECT_FALSE(cache.in_a1(2) && cache.in_a1out(2));
}