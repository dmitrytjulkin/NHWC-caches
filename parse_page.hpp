#pragma once

const int INIT_SIZE = 1000;

using page_t = char[INIT_SIZE];

page_t* slow_get_page(int key);
void process_page(int key);