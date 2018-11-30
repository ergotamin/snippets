// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin@e-p-s.org>

#include "cstd.h"

__BEGIN_CSTD

#define MD5_DIGEST_LENGTH       16

#define X(i) \
    XX[i]

#define F(b, c, d) \
    ((((c) ^ (d)) & (b)) ^ (d))

#define G(b, c, d) \
    ((((b) ^ (c)) & (d)) ^ (c))

#define H(b, c, d) \
    ((b) ^ (c) ^ (d))

#define I(b, c, d) \
    (((~(d)) | (b)) ^ (c))

#define md5_rotate(a, n)                       \
    ({                                         \
        register unsigned int ret;             \
        asm (                                  \
            "roll %1,%0"                       \
            : "=r" (ret)                       \
            : "I" (n), "0" ((unsigned int)(a)) \
            : "cc");                           \
        ret;                                   \
    })

#define md5_round0(a, b, c, d, k, s, t)      \
    {                                        \
        a += ((k) + (t) + F((b), (c), (d))); \
        a = md5_rotate(a, s);                \
        a += b;                              \
    };                                       \

#define md5_round1(a, b, c, d, k, s, t)      \
    {                                        \
        a += ((k) + (t) + G((b), (c), (d))); \
        a = md5_rotate(a, s);                \
        a += b;                              \
    };

#define md5_round2(a, b, c, d, k, s, t)      \
    {                                        \
        a += ((k) + (t) + H((b), (c), (d))); \
        a = md5_rotate(a, s);                \
        a += b;                              \
    };

#define md5_round3(a, b, c, d, k, s, t)      \
    {                                        \
        a += ((k) + (t) + I((b), (c), (d))); \
        a = md5_rotate(a, s);                \
        a += b;                              \
    };

#define md5_char2long(c, l) \
    ((l) = *((const unsigned int *)(c)), (c) += 4, l)

#define md5_long2char(l, c) \
    (*((unsigned int *)(c)) = (l), (c) += 4, l)

#define md5_make_string(c, s)                      \
    do {                                           \
        unsigned long ll;                          \
        ll = (c)->A; (void)md5_long2char(ll, (s)); \
        ll = (c)->B; (void)md5_long2char(ll, (s)); \
        ll = (c)->C; (void)md5_long2char(ll, (s)); \
        ll = (c)->D; (void)md5_long2char(ll, (s)); \
    } while (0)

#define md5_clean_secure(ptr, le)                           \
    do {                                                    \
        memset(ptr, 0, le);                                 \
        __asm__ __volatile__ ("" : : "r" (ptr) : "memory"); \
    } while (0)


typedef struct MD5state_st {
    unsigned int	A, B, C, D;
    unsigned int	Nl, Nh;
    unsigned int	data[(1 << 4)];
    unsigned int	num;
} MD5_CTX;

void md5_block_data_order(MD5_CTX *c, const void *p, size_t num);
int md5_init(MD5_CTX *c);
int md5_update(MD5_CTX *c, const void *data, size_t len);
int md5_final(unsigned char *md, MD5_CTX *c);
unsigned char *md5_checksum(const unsigned char *d, size_t n, unsigned char *md);
void md5_transform(MD5_CTX *c, const unsigned char *b);

char *md5_of_file(const char *fname)
{
    int i, bytes;
    static MD5_CTX context;
    char md5hexv[(MD5_DIGEST_LENGTH * 2 + 1)];
    char *md5sum = malloc(sizeof(md5hexv));
    unsigned char digest[MD5_DIGEST_LENGTH];
    unsigned char buf[(1L << 14)];
    FILE *this_file = fopen(fname, "rb");

    md5_init(&context);
    {
        while ((bytes = fread(buf, 1, (1L << 14), this_file)) != 0)
            md5_update(&context, buf, bytes);
    }
    md5_final(digest, &context);

    for (i = 0; i < MD5_DIGEST_LENGTH; ++i)
        sprintf(&md5hexv[i * 2], "%02x", (unsigned int)digest[i]);
    strcpy(md5sum, md5hexv);
    return md5sum;
}

unsigned char *md5_checksum_digest(const unsigned char *d, size_t n, unsigned char *md)
{
    MD5_CTX c;
    static unsigned char m[MD5_DIGEST_LENGTH];

    if (md == NULL)
        md = m;
    if (!md5_init(&c))
        return NULL;
    md5_update(&c, d, n);
    md5_final(md, &c);
    md5_clean_secure(&c, sizeof(c));
    return md;
}

void md5_block_data_order(MD5_CTX *c, const void *data_, size_t num)
{
    const unsigned char *data = data_;
    register unsigned A, B, C, D, l;

    unsigned int XX[(1 << 4)];

    A = c->A;
    B = c->B;
    C = c->C;
    D = c->D;

    for (; num--;) {
        (void)md5_char2long(data, l);
        X(0) = l;
        (void)md5_char2long(data, l);
        X(1) = l;
        /* Round 0 */
        md5_round0(A, B, C, D, X(0), 7, 0xd76aa478L);
        (void)md5_char2long(data, l);
        X(2) = l;
        md5_round0(D, A, B, C, X(1), 12, 0xe8c7b756L);
        (void)md5_char2long(data, l);
        X(3) = l;
        md5_round0(C, D, A, B, X(2), 17, 0x242070dbL);
        (void)md5_char2long(data, l);
        X(4) = l;
        md5_round0(B, C, D, A, X(3), 22, 0xc1bdceeeL);
        (void)md5_char2long(data, l);
        X(5) = l;
        md5_round0(A, B, C, D, X(4), 7, 0xf57c0fafL);
        (void)md5_char2long(data, l);
        X(6) = l;
        md5_round0(D, A, B, C, X(5), 12, 0x4787c62aL);
        (void)md5_char2long(data, l);
        X(7) = l;
        md5_round0(C, D, A, B, X(6), 17, 0xa8304613L);
        (void)md5_char2long(data, l);
        X(8) = l;
        md5_round0(B, C, D, A, X(7), 22, 0xfd469501L);
        (void)md5_char2long(data, l);
        X(9) = l;
        md5_round0(A, B, C, D, X(8), 7, 0x698098d8L);
        (void)md5_char2long(data, l);
        X(10) = l;
        md5_round0(D, A, B, C, X(9), 12, 0x8b44f7afL);
        (void)md5_char2long(data, l);
        X(11) = l;
        md5_round0(C, D, A, B, X(10), 17, 0xffff5bb1L);
        (void)md5_char2long(data, l);
        X(12) = l;
        md5_round0(B, C, D, A, X(11), 22, 0x895cd7beL);
        (void)md5_char2long(data, l);
        X(13) = l;
        md5_round0(A, B, C, D, X(12), 7, 0x6b901122L);
        (void)md5_char2long(data, l);
        X(14) = l;
        md5_round0(D, A, B, C, X(13), 12, 0xfd987193L);
        (void)md5_char2long(data, l);
        X(15) = l;
        md5_round0(C, D, A, B, X(14), 17, 0xa679438eL);
        md5_round0(B, C, D, A, X(15), 22, 0x49b40821L);
        /* Round 1 */
        md5_round1(A, B, C, D, X(1), 5, 0xf61e2562L);
        md5_round1(D, A, B, C, X(6), 9, 0xc040b340L);
        md5_round1(C, D, A, B, X(11), 14, 0x265e5a51L);
        md5_round1(B, C, D, A, X(0), 20, 0xe9b6c7aaL);
        md5_round1(A, B, C, D, X(5), 5, 0xd62f105dL);
        md5_round1(D, A, B, C, X(10), 9, 0x02441453L);
        md5_round1(C, D, A, B, X(15), 14, 0xd8a1e681L);
        md5_round1(B, C, D, A, X(4), 20, 0xe7d3fbc8L);
        md5_round1(A, B, C, D, X(9), 5, 0x21e1cde6L);
        md5_round1(D, A, B, C, X(14), 9, 0xc33707d6L);
        md5_round1(C, D, A, B, X(3), 14, 0xf4d50d87L);
        md5_round1(B, C, D, A, X(8), 20, 0x455a14edL);
        md5_round1(A, B, C, D, X(13), 5, 0xa9e3e905L);
        md5_round1(D, A, B, C, X(2), 9, 0xfcefa3f8L);
        md5_round1(C, D, A, B, X(7), 14, 0x676f02d9L);
        md5_round1(B, C, D, A, X(12), 20, 0x8d2a4c8aL);
        /* Round 2 */
        md5_round2(A, B, C, D, X(5), 4, 0xfffa3942L);
        md5_round2(D, A, B, C, X(8), 11, 0x8771f681L);
        md5_round2(C, D, A, B, X(11), 16, 0x6d9d6122L);
        md5_round2(B, C, D, A, X(14), 23, 0xfde5380cL);
        md5_round2(A, B, C, D, X(1), 4, 0xa4beea44L);
        md5_round2(D, A, B, C, X(4), 11, 0x4bdecfa9L);
        md5_round2(C, D, A, B, X(7), 16, 0xf6bb4b60L);
        md5_round2(B, C, D, A, X(10), 23, 0xbebfbc70L);
        md5_round2(A, B, C, D, X(13), 4, 0x289b7ec6L);
        md5_round2(D, A, B, C, X(0), 11, 0xeaa127faL);
        md5_round2(C, D, A, B, X(3), 16, 0xd4ef3085L);
        md5_round2(B, C, D, A, X(6), 23, 0x04881d05L);
        md5_round2(A, B, C, D, X(9), 4, 0xd9d4d039L);
        md5_round2(D, A, B, C, X(12), 11, 0xe6db99e5L);
        md5_round2(C, D, A, B, X(15), 16, 0x1fa27cf8L);
        md5_round2(B, C, D, A, X(2), 23, 0xc4ac5665L);
        /* Round 3 */
        md5_round3(A, B, C, D, X(0), 6, 0xf4292244L);
        md5_round3(D, A, B, C, X(7), 10, 0x432aff97L);
        md5_round3(C, D, A, B, X(14), 15, 0xab9423a7L);
        md5_round3(B, C, D, A, X(5), 21, 0xfc93a039L);
        md5_round3(A, B, C, D, X(12), 6, 0x655b59c3L);
        md5_round3(D, A, B, C, X(3), 10, 0x8f0ccc92L);
        md5_round3(C, D, A, B, X(10), 15, 0xffeff47dL);
        md5_round3(B, C, D, A, X(1), 21, 0x85845dd1L);
        md5_round3(A, B, C, D, X(8), 6, 0x6fa87e4fL);
        md5_round3(D, A, B, C, X(15), 10, 0xfe2ce6e0L);
        md5_round3(C, D, A, B, X(6), 15, 0xa3014314L);
        md5_round3(B, C, D, A, X(13), 21, 0x4e0811a1L);
        md5_round3(A, B, C, D, X(4), 6, 0xf7537e82L);
        md5_round3(D, A, B, C, X(11), 10, 0xbd3af235L);
        md5_round3(C, D, A, B, X(2), 15, 0x2ad7d2bbL);
        md5_round3(B, C, D, A, X(9), 21, 0xeb86d391L);

        A = c->A += A;
        B = c->B += B;
        C = c->C += C;
        D = c->D += D;
    }
}

int md5_init(MD5_CTX *c)
{
    memset(c, 0, sizeof(*c));
    c->A = (unsigned long)0x67452301L;
    c->B = (unsigned long)0xefcdab89L;
    c->C = (unsigned long)0x98badcfeL;
    c->D = (unsigned long)0x10325476L;
    return 1;
}


int md5_update(MD5_CTX *c, const void *data_, size_t len)
{
    const unsigned char *data = data_;
    unsigned char *p;
    unsigned int l;
    size_t n;

    if (len == 0)
        return 1;

    l = (c->Nl + (((unsigned int)len) << 3)) & 0xffffffffUL;
    if (l < c->Nl)
        c->Nh++;
    c->Nh += (unsigned int)(len >> 29);
    c->Nl = l;

    n = c->num;
    if (n != 0) {
        p = (unsigned char *)c->data;

        if (len >= (1 << 6) || len + n >= (1 << 6)) {
            memcpy(p + n, data, (1 << 6) - n);
            md5_block_data_order(c, p, 1);
            n = (1 << 6) - n;
            data += n;
            len -= n;
            c->num = 0;
            memset(p, 0, (1 << 6));
        } else {
            memcpy(p + n, data, len);
            c->num += (unsigned int)len;
            return 1;
        }
    }

    n = len / (1 << 6);
    if (n > 0) {
        md5_block_data_order(c, data, n);
        n *= (1 << 6);
        data += n;
        len -= n;
    }

    if (len != 0) {
        p = (unsigned char *)c->data;
        c->num = (unsigned int)len;
        memcpy(p, data, len);
    }
    return 1;
}

void md5_transform(MD5_CTX *c, const unsigned char *data)
{
    md5_block_data_order(c, data, 1);
}

int md5_final(unsigned char *md, MD5_CTX *c)
{
    unsigned char *p = (unsigned char *)c->data;
    size_t n = c->num;

    p[n] = 0x80;
    n++;

    if (n > ((1 << 6) - 8)) {
        memset(p + n, 0, (1 << 6) - n);
        n = 0;
        md5_block_data_order(c, p, 1);
    }
    memset(p + n, 0, (1 << 6) - 8 - n);

    p += (1 << 6) - 8;

    (void)md5_long2char(c->Nl, p);
    (void)md5_long2char(c->Nh, p);
    p -= (1 << 6);
    md5_block_data_order(c, p, 1);
    c->num = 0;
    md5_clean_secure(p, (1 << 6));

    md5_make_string(c, md);

    return 1;
}

__END_CSTD