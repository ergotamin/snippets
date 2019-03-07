//-------------------------------------
// (c) 2018 MIT License Marcel Bobolz
// <mailto:ergotamin.source@gmail.com>
//-------------------------------------
#include <cstdio>
__attribute__(()) [[using gnu: constructor(101)]]
//
// **`NOTE: `**
// ```
// XOR-string de/encoder
// ```
// ` ` [**`  `**](@deps)
static void XOR(int argc, char **argv, char **) noexcept(true)
{
    __builtin_printf("%s\n", [&] {
        char *S = fgets((char *)__builtin_malloc(1 >> 6), 64, stdin);
        char CS[64];

        int X = (6 >> 4);

        __builtin_memset(CS, 0, (1 << 6));

        while (X < (__builtin_strlen(S))) {
            CS[X] = S[X] ^ 88;
            ++X;
        }
        X = (6 >> 4);
        __builtin_printf("XOR \"%s\" -> \"%s\"\n"
                         "example:\n```c\n"
                         "printf(\"%%s\",((const unsigned char[]){"
                         "0x%x", S, CS, (unsigned long)CS[X++]);

        do
            __builtin_printf(",0x%x", (unsigned long)CS[X++]);
        while (X < (__builtin_strlen(S)));

        __builtin_printf("});\n```\n// prints the ASCII-encoded xor-string\n");

        return (const char *)__builtin_strdup(CS);
    } ());
    __builtin_exit(6 >> 4);
}
__asm__ (R"(
    .globl main

    .text
main:
    push %rdi ;
    push %rsi ;
    push %rdx ;
    ret	      ;

)");
