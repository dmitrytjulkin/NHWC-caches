#include "Beladi.hpp"

namespace BeladyCache {

    template<typename PageT, typename KeyT>
    BeladyCache<PageT, KeyT>::BeladyCache
            (std::size_t capacity_in, const std::vector<KeyT>& requests) 
                : capacity (capacity_in), hits (0), faults (0){
        PrepareFuture (requests);
    }


    template<typename PageT, typename KeyT> 
    bool BeladyCache<PageT, KeyT>::IsFull () const {
        return Pages.size () >= capacity;
    }

    template<typename PageT, typename KeyT> 
    void BeladyCache<PageT, KeyT>::PrepareFuture (const std::vector<KeyT>& requests){
        const std::size_t size = requests.size ();

        std::unordered_map<KeyT, std::size_t> LastUse;
        NextUse.assign (size, NONE);
        LastUse.reserve (size);

        for (std::size_t index = size; index > 0; --index){
            std::size_t i = index - 1;
            auto it = LastUse.find (requests[i]);
            if (it != LastUse.end ())
                NextUse[i] = it->second;

            LastUse[requests[i]] = i;
        }
    }


    template<typename PageT, typename KeyT>
    void BeladyCache<PageT, KeyT>::Simulate (const std::vector<KeyT>& requests) {

        Pages.clear ();
        Cursor.clear ();
        Hash.clear ();

        hits = 0;
        faults = 0;

        PrepareFuture (requests);

        if (capacity == 0) {
            faults = requests.size();
            return;
        }

        for (std::size_t i = 0; i < requests.size (); ++i) {
            const KeyT& page = requests[i];

            // Проверяем, есть ли страница в кэше
            HashIt hit = Hash.find (page);

            if (hit != Hash.end ()) {
                ++hits;

                // Обновляем следующее обращение к странице
                Cursor[page] = NextUse[i];

                continue;
            }

            ++faults;

            // Если кэш ещё не заполнен
            if (!IsFull ()) {

                Pages.push_back (page);

                ListIt it = Pages.end ();
                --it;

                Hash[page] = it;
                Cursor[page] = NextUse[i];

                continue;
            }

            // Ищем страницу, которая понадобится позже всех
            ListIt victim = Pages.end ();
            std::size_t farthest = 0;

            for (ListIt it = Pages.begin  (); it != Pages.end (); ++it) {
                std::size_t next = Cursor[*it];

                if (next == NONE) {
                    victim = it;
                    break;
                }

                if (victim == Pages.end () || next > farthest) {
                    farthest = next;
                    victim = it;
                }
            }

            // Удаляем выбранную страницу
            KeyT victimPage = *victim;

            Hash.erase (victimPage);
            Cursor.erase (victimPage);
            Pages.erase (victim);

            // Добавляем новую страницу
            Pages.push_back (page);

            ListIt it = Pages.end ();
            --it;

            Hash[page] = it;
            Cursor[page] = NextUse[i];
        }
    }
    

    template<typename PageT, typename KeyT>
    std::size_t BeladyCache<PageT, KeyT>::GetHits () const {
        return hits;
    }

    template<typename PageT, typename KeyT>
    std::size_t BeladyCache<PageT, KeyT>::GetFaults () const {
        return faults;
    }

} // namespace BeladyCache