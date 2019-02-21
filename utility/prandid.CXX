// !c++
// #pragma once
//-------------------------------------
// (c) 2018 MIT License Marcel Bobolz
// <mailto:ergotamin.source@gmail.com>
//-------------------------------------
#
// [**`prandid`**](github.com/ergotamin/snippets)
// **`{`** ``` __implementation__ ``` **`}`**
unsigned short prandid(void)
{
    int32_t partial = 0x008000;
    int32_t useable = 0x7FFF7FFF;
    int32_t id = 0x7FFFFFFF;
    uint64_t nseed = 0;
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    nseed = (uint64_t)ts.tv_sec * 0x3B9ACA00UL + (uint64_t)ts.tv_nsec;
    srand(nseed);

    do
        id = rand();
    while (id > useable);
    return (unsigned short)(id / partial);
}
//
