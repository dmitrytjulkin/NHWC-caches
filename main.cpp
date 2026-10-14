#include <iostream>
#include <vector>
#include <optional>

#include "get_params.h"
#include "slow_get_page.h"
#include "2Q.h"

int main()
{
    int hit_num = 0, miss_num = 0;
    std::optional<InputData> input = Reading();

    if (input == std::nullopt)
        return 1;

    Cache_2q_t<int> cache(input->cache_size);

    for(size_t i = 0; i < input->data.size(); i++)
    {
        if (cache.lookup_update(input->data[i], slow_get_page))
            hit_num++;
        else
            miss_num++;
    }

    std::cout << "Number of hits = " << hit_num << "\n";
    std::cout << "Number of misses = " << miss_num << "\n";

    return 0;
}