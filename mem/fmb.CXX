// !c++
// #pragma once
//-------------------------------------
// (c) 2018 MIT License Marcel Bobolz
// <mailto:ergotamin.source@gmail.com>
//-------------------------------------
#
// [**`fmb`**](github.com/ergotamin/snippets)
// **`{`** ``` __implementation__ ``` **`}`**
void fmb(void)
{
    asm volatile ("mfence" : : : "memory");
    __sync_synchronize();
}
//
