#include <iostream>
#include <vector>
#include <optional>

#include "get_params.h"

int main()
{
    std::optional<InputData> input = Reading();

    if (input == std::nullopt)
        return 1;

    return 0;
}