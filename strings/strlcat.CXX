// !c++
// #pragma once
//-------------------------------------
// (c) 2018 MIT License Marcel Bobolz
// <mailto:ergotamin.source@gmail.com>
//-------------------------------------
#
// [**`strlcat`**](github.com/ergotamin/snippets)
// **`{`** ``` __implementation__ ``` **`}`**
long unsigned int strlcat(char *dst, const char *src, long unsigned int sz)
{
    const char *offdst = dst;
    const char *offsrc = src;
    long unsigned int n = sz;
    long unsigned int dstlen;

    while (n-- != 0 && *dst != '\0') dst++;
    dstlen = dst - offdst;
    n = sz - dstlen;

    if (n-- == 0) return dstlen + strlen(src);

    while (*src != '\0') {
        if (n != 0) {
            *dst++ = *src;
            n--;
        }
        src++;
    }
    *dst = '\0';

    return dstlen + (src - offsrc) + 1;
}
//
