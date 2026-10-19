#ifndef GET_PARAMS
#define GET_PARAMS

#include <vector>
#include <optional>
#include <string>

struct InputData {
    int total_cache_size = 0;
    std::vector<std::string> levels_types;
    std::vector<int> data;
};

std::optional<InputData> Reading();

#endif