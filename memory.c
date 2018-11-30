// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin@e-p-s.org>


#include "cstd.h"

__BEGIN_CSTD

void *alloc(long unsigned int bytes)
{
    void *mem = (void *)0;

    mem = malloc(sizeofptr * bytes);
    memset(mem, 0, bytes * sizeofptr);
    if (mem != (void *)0) {
        return mem;
    } else {
        cerror("alloc() - memory allocation failed !");
        abort();
    }
}

void *nalloc(long unsigned int nblocks, long unsigned int nbytes)
{
    void *mem = (void *)0;

    mem = calloc(1, (nblocks * nbytes));

    if (mem != (void *)0) {
        return mem;
    } else {
        cerror("nalloc() - memory allocation failed !");
        abort();
    }
}

void memfree(void *ptr)
{
    (ptr == (void *)0) ? (nop()) : (free(ptr));
    __asm__ __volatile__ ("mov 0, %0" : : "r" (ptr) : "memory");
}

__END_CSTD