#include <iostream>
#include <vector>
#include <optional>

#include "get_params.hpp"
#include "slow_get_page.hpp"
#include "level_sizes.hpp"
#include "ICache.hpp"
#include "Factory.hpp"

int main()
{
    int hit_num = 0, miss_num = 0;
    std::optional<InputData> input = Reading();

    if (input == std::nullopt)
        return 1;

    std::vector<int> level_sizes = Count_Levels_Sizes(input);

    std::vector<std::unique_ptr<ICache<int, int>>> levels(input->levels_types.size());
    
    CacheFactory<int, int> factory;

    for (int i = 0; i < levels.size(); i++)
        levels[i] = factory.create(input->levels_types[i], level_sizes[i]);

    std::function<int(int)> fetcher = [](int key) {
        return slow_get_page(key);
    };

    for (int i = levels.size() - 2; i >= 0; --i) {
        fetcher = [&levels, i, fetcher](int key) -> int {
            levels[i+1]->lookup_update(key, fetcher);
            return fetcher(key);
        };
    }

    std::cout << "Number of hits = " << hit_num << "\n";
    std::cout << "Number of misses = " << miss_num << "\n";

    return 0;
}