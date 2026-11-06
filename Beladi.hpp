#pragma once

#include <limits>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <list>
#include <cstddef>
#include <limits>

namespace BeladyCache {

    template<typename PageT, typename KeyT> 
    struct BeladyCache {
    public:
        explicit BeladyCache (std::size_t capacity_in, const std::vector<KeyT>& requests);

        void Simulate (const std::vector<KeyT>& requests);

        std::size_t GetHits () const;
        std::size_t GetFaults () const;

        bool IsFull () const;
        void PrepareFuture (const std::vector<KeyT>& requests);

    private:
        std::size_t capacity;
        std::size_t hits;
        std::size_t faults;

        std::list<KeyT> Pages;
        using ListIt = typename std::list<KeyT>::iterator;
        using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;

        std::vector<std::size_t> NextUse; // Массив следущего входа эллементов
        std::unordered_map<KeyT, std::size_t> Cursor; //Соответсвие Страницы и следущего входа

        std::unordered_map<KeyT, ListIt> Hash;
    };

    static constexpr std::size_t NONE = std::numeric_limits<std::size_t>::max ();

} // namespace BeladyCache