#include<iostream>

#include"debug.hpp"

void print_LFU_list (std::list<LFU_content<PAGE_T>> list, int size)
{
    auto it = list.begin();
    for (int num = 0; num != size; ++num) {
        if (num >= list.size())
            std::cout << "| 0 |";
        
        else
            std:: cout << "| " << it->key << " |";
        ++it;
    }

    std::cout << "\n";
}
