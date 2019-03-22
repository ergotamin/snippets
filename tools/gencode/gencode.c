// !c++
//      ------------------------------
//              MIT-License 0x7e3
//       <ergotamin.source@gmail.com>
//      ------------------------------
//
#include <stdio.h>
#include <sys/stat.h>
//
#define BEG                                    \
    "//      ------------------------------\n" \
    "//              MIT-License 0x7e3\n"      \
    "//       <ergotamin.source@gmail.com>\n"  \
    "//      ------------------------------\n" \
    "#if defined(__cplusplus)\n"               \
    "extern \"C\" {\n"                         \
    "#endif\n"                                 \
    "const struct {\n"                         \
    "\tunsigned long size;\n"                  \
    "\tstatic unsigned char data[%lu]"         \
    "__attribute__((aligned(%u)));\n"          \
    "} %s_blob = { %lu,\n"                     \
    "{\n"
//
#define END                      \
    "}\n"                        \
    "};\n"                       \
    "#if defined(__cplusplus)\n" \
    "}\n"                        \
    "#endif\n"                   \
    "\n"
//
extern const char *__progname;
//
int main(int argc, char **argv)
{
    --argc;
    if (1 == argc) {
        if (0 != __builtin_strcmp("-", argv[argc])) {
            //
            struct stat s;

            if (0 != stat(argv[argc], &s)) {
                //
                __builtin_printf("%s", "stat() failed ...\n");
                __builtin_exit(1);
            }

            //
            FILE *f = NULL;

            if (NULL == (f = fopen(argv[argc], "rb"))) {
                //
                __builtin_printf("%s", "fopen() failed ...\n");
                __builtin_exit(2);
            }

            //
            unsigned char *data =
                (unsigned char *)__builtin_calloc(
                    sizeof(unsigned char), s.st_size + 1);

            if (fread((void *)data, sizeof(unsigned char), s.st_size, f) != s.st_size) {
                //
                fclose(f);
                __builtin_printf("%s", "fread() failed ...\n");
                __builtin_exit(3);
            }

            fflush(f);
            fclose(f);
            data[s.st_size + 1] = 0b0000;
            //
            char *tr = NULL;
            char *of = __builtin_strdup(argv[argc]);

            if (NULL != (tr = __builtin_strrchr(of, '/'))) {
                *tr = 0;
                of = (tr + 1);
            }

            if (NULL != (tr = __builtin_strrchr(of, '.')))
                *tr = 0;

            __builtin_strcat(of, ".c");
            //
            f = NULL;

            if (NULL == (f = fopen(of, "wb"))) {
                //
                __builtin_printf("%s", "fopen() failed ...\n");
                __builtin_exit(2);
            }

            tr = __builtin_strrchr(of, '.');
            *tr = 0;
            //
            unsigned int c = 1U;
            unsigned long n = 0UL;

            fprintf(f, BEG, s.st_size + 1, sizeof(unsigned char), of, s.st_size + 1);
            fflush(f);

            for (; n <= s.st_size; n++) {
                fprintf(f, "0x%.2x,\0", data[n]);
                if (!(c % 8)) {
                    fputc('\n', f);
                    c = 1U;
                } else {
                    c++;
                }
            }

            fflush(f);
            fprintf(f, END);
            fclose(f);
            //
            __builtin_free((void *)data);
        } else {
            __builtin_printf("cannot determine filename from stdin...\n");
            __builtin_exit(4);
        }
        return 0;
    } else {
        __builtin_printf("Usage:\n\t%s <filename>\n\n\t" \
                         "generates <filename>.c"        \
                         " which contains <filename> "   \
                         "as c-byte-array.\n");
    }
    return 1;
}
