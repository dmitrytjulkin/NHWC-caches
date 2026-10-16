#include <gtest/gtest.h>

#include "ARC.hpp"
#include "../slow_get_page.hpp"


TEST(ARCBasic, MissOnEmpty) {
    Cache_arc_t<int> cache(4);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
}

TEST(ARCBasic, FirstMissThenHit) {
    Cache_arc_t<int> cache(4);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_TRUE(cache.lookup_update(1, slow_get_page));
}

TEST(ARCBasic, DifferentKeysAllMiss) {
    Cache_arc_t<int> cache(4);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(2, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(3, slow_get_page));
}

TEST(ARCBasic, RepeatAfterLongSequence) {
    Cache_arc_t<int> cache(4);
    for (int i = 1; i <= 20; i++)
        cache.lookup_update(i, slow_get_page);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
}


TEST(ARCState, FirstInsertGoesToT1) {
    Cache_arc_t<int> cache(8);
    cache.lookup_update(42, slow_get_page);
    EXPECT_TRUE(cache.in_t1(42));
    EXPECT_FALSE(cache.in_t2(42));
    EXPECT_FALSE(cache.in_b1(42));
    EXPECT_FALSE(cache.in_b2(42));
}

TEST(ARCState, HitInT1PromotesToT2) {
    Cache_arc_t<int> cache(8);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    EXPECT_FALSE(cache.in_t1(1));
    EXPECT_TRUE(cache.in_t2(1));
}

TEST(ARCState, HitInT2StaysInT2) {
    Cache_arc_t<int> cache(8);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    EXPECT_FALSE(cache.in_t1(1));
    EXPECT_TRUE(cache.in_t2(1));
}

TEST(ARCState, SecondInsertGoesToT1) {
    Cache_arc_t<int> cache(8);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    EXPECT_TRUE(cache.in_t1(1));
    EXPECT_TRUE(cache.in_t1(2));
    EXPECT_FALSE(cache.in_t2(1));
    EXPECT_FALSE(cache.in_t2(2));
}


TEST(ARCGhost, B1HitPromotesToT2) {
    Cache_arc_t<int> cache(4);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(3, slow_get_page);
    cache.lookup_update(4, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(5, slow_get_page);

    EXPECT_TRUE(cache.in_b1(2));

    EXPECT_FALSE(cache.lookup_update(2, slow_get_page));
    EXPECT_FALSE(cache.in_b1(2));
    EXPECT_TRUE(cache.in_t2(2));
}

TEST(ARCGhost, GhostHitIsMiss) {
    Cache_arc_t<int> cache(4);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(3, slow_get_page);
    cache.lookup_update(4, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(5, slow_get_page);

    EXPECT_FALSE(cache.lookup_update(2, slow_get_page));
}


TEST(ARCEviction, T1OverflowGoesToB1) {
    Cache_arc_t<int> cache(4);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(3, slow_get_page);
    cache.lookup_update(4, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(5, slow_get_page);

    EXPECT_TRUE(cache.in_b1(2));
    EXPECT_FALSE(cache.in_t1(2));
}

TEST(ARCEviction, B2FillsFromT2) {
    Cache_arc_t<int> cache(4);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(3, slow_get_page);
    cache.lookup_update(4, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(3, slow_get_page);
    cache.lookup_update(4, slow_get_page);
    cache.lookup_update(5, slow_get_page);

    EXPECT_TRUE(cache.in_b2(1));
}


TEST(ARCBoundary, CacheSizeOne) {
    Cache_arc_t<int> cache(1);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(2, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(3, slow_get_page));
}

TEST(ARCBoundary, CacheSizeOneWithReuse) {
    Cache_arc_t<int> cache(1);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_TRUE(cache.lookup_update(1, slow_get_page));
}

TEST(ARCBoundary, CacheSizeTwo) {
    Cache_arc_t<int> cache(2);
    EXPECT_FALSE(cache.lookup_update(1, slow_get_page));
    EXPECT_FALSE(cache.lookup_update(2, slow_get_page));
    EXPECT_TRUE(cache.lookup_update(1, slow_get_page));
    EXPECT_TRUE(cache.lookup_update(1, slow_get_page));
}


TEST(ARCPattern, SameKeyRepeated) {
    Cache_arc_t<int> cache(4);
    EXPECT_FALSE(cache.lookup_update(7, slow_get_page));
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(cache.lookup_update(7, slow_get_page)) << "iteration " << i;
    }
}

TEST(ARCPattern, SequentialScanAllMiss) {
    Cache_arc_t<int> cache(4);
    for (int i = 1; i <= 20; i++) {
        EXPECT_FALSE(cache.lookup_update(i, slow_get_page)) << "i = " << i;
    }
}

TEST(ARCPattern, HotKeySurvivesScan) {
    Cache_arc_t<int> cache(8);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    EXPECT_TRUE(cache.in_t2(1));

    for (int i = 100; i < 120; i++)
        cache.lookup_update(i, slow_get_page);

    EXPECT_TRUE(cache.in_t2(1));
    EXPECT_TRUE(cache.lookup_update(1, slow_get_page));
}

TEST(ARCPattern, HotKeyAlternating) {
    Cache_arc_t<int> cache(4);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    for (int i = 100; i < 120; i++) {
        cache.lookup_update(i, slow_get_page);
        cache.lookup_update(1, slow_get_page);
    }
    EXPECT_TRUE(cache.in_t2(1));
}


TEST(ARCInvariant, KeyNotInBothT1AndT2) {
    Cache_arc_t<int> cache(8);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(3, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);

    for (int k = 1; k <= 3; k++) {
        EXPECT_FALSE(cache.in_t1(k) && cache.in_t2(k)) << "key " << k;
    }
}

TEST(ARCInvariant, KeyNotInBothRealAndGhost) {
    Cache_arc_t<int> cache(4);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(2, slow_get_page);
    cache.lookup_update(3, slow_get_page);
    cache.lookup_update(4, slow_get_page);
    cache.lookup_update(1, slow_get_page);
    cache.lookup_update(5, slow_get_page);

    for (int k = 1; k <= 5; k++) {
        EXPECT_FALSE(cache.in_t1(k) && cache.in_b1(k)) << "key " << k << " T1+B1";
        EXPECT_FALSE(cache.in_t2(k) && cache.in_b2(k)) << "key " << k << " T2+B2";
    }
}