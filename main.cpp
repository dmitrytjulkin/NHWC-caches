#include <iostream>
#include <vector>
#include <cassert>

#include "Beladi.hpp"
#include "Beladi.cpp"

int main() {

    { // TEST 1
        std::cout << "Program started\n";
        std::vector<int> requests = {1, 2, 1, 2};

        BeladyCache::BeladyCache<int, int> cache(2, requests);
        cache.Simulate(requests);

        assert(cache.GetHits() == 2);
        assert(cache.GetFaults() == 2);

        std::cout << "Test 1 passed\n";
    }

    { //TEST 2
        std::vector<int> requests = {1, 2, 3, 4};

        BeladyCache::BeladyCache<int, int> cache(2, requests);
        cache.Simulate(requests);

        assert(cache.GetHits() == 0);
        assert(cache.GetFaults() == 4);

        std::cout << "Test 2 passed\n";
    }

    { //TEST 3
        std::vector<int> requests = {1, 2, 3, 1};

        BeladyCache::BeladyCache<int, int> cache(2, requests);
        cache.Simulate(requests);

        assert(cache.GetHits() == 1);
        assert(cache.GetFaults() == 3);

        std::cout << "Test 3 passed\n";
    }

    { // TEST 4
        std::vector<int> requests = {1, 2, 3};

        BeladyCache::BeladyCache<int, int> cache(0, requests);
        cache.Simulate(requests);

        assert(cache.GetHits() == 0);
        assert(cache.GetFaults() == 3);

        std::cout << "Test 4 passed\n";
    }

    { // TEST 5
        std::vector<int> requests = {};

        BeladyCache::BeladyCache<int, int> cache(2, requests);
        cache.Simulate(requests);

        assert(cache.GetHits() == 0);
        assert(cache.GetFaults() == 0);

        std::cout << "Test 5 passed\n";
    }

    // //TEST 6
    {
        std::vector<int> requests1 = {1, 2, 3};
        std::vector<int> requests2 = {1, 1, 1};

        BeladyCache::BeladyCache<int, int> cache(2, requests1);

        cache.Simulate(requests1);
        cache.Simulate(requests2);

        assert(cache.GetHits() == 2);
        assert(cache.GetFaults() == 1);

        std::cout << "Test 6 passed\n";
    }

    { //TEST 7
        std::vector<int> requests = {1, 2, 3, 2, 1};

        BeladyCache::BeladyCache<int, int> cache(2, requests);
        cache.Simulate(requests);

        assert(cache.GetHits() == 1);
        assert(cache.GetFaults() == 4);

        std::cout << "Test 7 passed\n";
    }

    std::cout << "\nAll tests passed!\n";

    return 0;
}