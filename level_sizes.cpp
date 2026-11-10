#include "level_sizes.hpp"
#include "get_params.hpp"

#include <vector>
#include <algorithm>
#include <optional>

std::vector<int> Count_Levels_Sizes(std::optional<InputData> input)
{
    int cache_num = input->levels_types.size();
    std::vector<int> levels_size(cache_num, 1);

    int denominator = cache_num * (cache_num + 1) / 2;
    int rem = input->total_cache_size - cache_num;
    int sum = 0;

    for (int i = 0; i < cache_num; i++) {
        levels_size[i] += std::max(((i + 1) * rem) / denominator, 0);
        sum += levels_size[i];
    }

    levels_size[cache_num - 1] += input->total_cache_size - sum;

    return levels_size;
}