#include<list>
#include<unordered_map>

int evict_if_needed(cache_t* cache, page_t* page_requests)
{
    int page_num = 0;
    int newpage_id = 0;

    while (page_requests[page_num] != NULL) {
        newpage_id = page_requests[page_num];

        if (!is_page_frequent(cache, newpage_id)) {
            ++page_num;
            continue;
        }

        bool page_cached = is_page_cached(cache, newpage_id)
        if (is_cache_full(cache) && page_cached) evict_last_page(cache);
        if (page_cached) slow_get_page(newpage_id);
        push_front(cache, newpage_id);

        process_page(newpage_id);

        ++page_num;
    }

    return 0; // count of hits
}
