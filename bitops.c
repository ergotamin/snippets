// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin.source@gmail.com>

__BEGIN_DECL

unsigned int chk(void *addr32, unsigned int bit)
{
    return (*((unsigned int *)addr32) >> bit) & 1;
}

void set(void *addr32, unsigned int bit)
{
    *((unsigned int *)addr32) |= (1 << bit);
}

void clr(void *addr32, unsigned int bit)
{
    *((unsigned int *)addr32) &= ~(1 << bit);
}

void tgl(void *addr32, unsigned int bit)
{
    *((unsigned int *)addr32) ^= (1 << bit);
}

void b2b(void *addr32, unsigned int from, int to)
{
    *((unsigned int *)addr32) ^= (-to & *((unsigned int *)addr32)) & (1 << from);
}

__END_DECL
