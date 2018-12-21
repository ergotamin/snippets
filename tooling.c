// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin.source@gmail.com>

__BEGIN_DECL

extern const char *__progname;

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

__END_DECL
