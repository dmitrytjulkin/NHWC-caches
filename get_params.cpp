#include "get_params.h"

#include <iostream>
#include <vector>
#include <optional>

std::optional<InputData> Reading()
{
    int data_size = 0;
    InputData input{};
    std::cout << "Enter cache size and data size: ";
    if (!(std::cin >> input.cache_size >> data_size))
    {
        std::cerr << "ERROR: couldn’t read the data.\n";
        return std::nullopt;
    }

    input.data.resize(data_size);

    std::cout << "Enter data: ";
    for (int i = 0; i < data_size; i++)
    {
        if (!(std::cin >> input.data[i]))
        {
            if (std::cin.eof())
                std::cerr << "ERROR: the data is less than what was stated.\n";
            else if (std::cin.fail())
                std::cerr << "ERROR: couldn’t read the data.\n";

            return std::nullopt;
        }
    }
    
    return input;
}