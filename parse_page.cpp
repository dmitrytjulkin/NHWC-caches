#include<stdio.h>
#include<unistd.h>

#include"LFU.h"

void slow_get_page(keyT key)
{
    sleep(4);

    return;
}

void process_page(keyT key)
{
    sleep(0.1);

    return;
}