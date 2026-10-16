#pragma once

#include<list>
#include"LFU/LFU.hpp"

#define GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

void print_LFU_list (std::list<LFU_content<PAGE_T>> list, int size);