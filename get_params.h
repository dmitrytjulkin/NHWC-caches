#ifndef GET_PARAMS
#define GET_PARAMS

#include <vector>
#include <optional>

struct InputData {
    int cache_size = 0;
    std::vector<int> data;
};

std::optional<InputData> Reading();

#endif