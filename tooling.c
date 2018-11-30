// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin@e-p-s.org>

#include "cstd.h"

__BEGIN_CSTD

extern const char *__progname;

void proginfo(char *desc)
{
    cprint(bold()                                     \
           "\n\t"                                     \
           smul()                                     \
           fg("0", "225", "50")                       \
           "'%s':\n"                                  \
           sgr()                                      \
           bold()                                     \
           "\tAuthor:\n"                              \
           fg("40", "255", "40")                      \
           "\t\t"AUTHORNAME "\n\t\t"AUTHORMAIL "\n\n" \
           sgr()                                      \
           bold()                                     \
           "\tDescription:\n"                         \
           sitm()                                     \
           "\t\t" "%s"                                \
           sgr()                                      \
           "\n\n\t\t\t\t"                             \
           fg("0", "255", "125")                      \
           ""LICENSE "\n"                             \
           sgr(),                                     \
           __progname,                                \
           desc);
}

void *nop(void)
{
    return NULL;
}

long int randombytes(long unsigned int le)
{
    long int x = 0;

    getentropy((void *)&x, le);
    return (long unsigned int)x;
}

int sysspawn(const char *fmt, ...)
{
    va_list args;
    char *buf = alloc(kilobyte(4) * sizeofptr);

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    if (buf) return system(buf);
    else return FAILURE;
}

__END_CSTD
