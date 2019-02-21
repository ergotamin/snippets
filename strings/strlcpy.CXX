// !c++
// #pragma once
//-------------------------------------
// (c) 2018 MIT License Marcel Bobolz
// <mailto:ergotamin.source@gmail.com>
//-------------------------------------
// #include <cstddef>
// #include <cstdlib>
// #include <cstdio>
#
// [**`strlcpy`**](github.com/ergotamin/snippets)
// **`{`** ``` __implementation__ ``` **`}`**
long unsigned int strlcpy(char *dst, const char *src, long unsigned int sz)
{
    const char *osrc = src;
    long unsigned int bytes = sz;

    if (bytes != 0)
        while (--bytes != 0)
            if ((*dst++ = *src++) == '\0') break;

    if (bytes == 0) {
        if (sz != 0) *dst = '\0';

        while (*src++);
    }
    return src - osrc - 1;
}
//
