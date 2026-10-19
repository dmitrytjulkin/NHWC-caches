#include "get_params.hpp"

#include <iostream>
#include <vector>
#include <optional>

std::optional<InputData> Reading()
{
    int cache_num = 0, data_size = 0;
    InputData input{};
    if (!(std::cin >> cache_num)) {
        std::cerr << "ERROR: couldn’t read the number of caches.\n";
        return std::nullopt;
    }

    input.levels_types.resize(cache_num);

    for (int i = 0; i < cache_num; i++)
    {
        if (!(std::cin >> input.levels_types[i])) {
            std::cerr << "ERROR: couldn’t read the data.\n";
            return std::nullopt;
        }
    }

    if (!(std::cin >> input.total_cache_size >> data_size)) {
        std::cerr << "ERROR: couldn’t read sizes.\n";
        return std::nullopt;
    }

    input.data.resize(data_size);

    for (int i = 0; i < data_size; ++i) {
        if (!(std::cin >> input.data[i])) {
            if (std::cin.eof())
                std::cerr << "ERROR: the data is less than what was stated.\n";
            else if (std::cin.fail())
                std::cerr << "ERROR: couldn’t read the data.\n";

            return std::nullopt;
        }
    }
    
    return input;
}