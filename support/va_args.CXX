// !c++
template<typename CharT>
static char *va_args_fn(const CharT *std)
{
    ; // do stuff
    return "Stuff Done";
}
//
template<typename CharT, typename ... Vargs>
static char *va_args_fn(const CharT *std, const CharT *cur, Vargs... nargs)
{
    ;   // do stuff with std and cur (current va_arg)
    ;   // recursive iteration with (... nargs)
    return "Stuff Done";
}
// c99
#include <stdarg.h>
unsigned int va_args_fn(int first, int last, ...)
{
    va_list args;

    va_start(args, last);
    const char *va_param = va_arg(args, const char *);
    va_end(args);
    return last;
}
//
unsigned int get_opt(const char *arg, int num, ...)
{
    va_list args;
    int r = -1;
    char *def = NULL;
    unsigned int opt = 0;

    va_start(args, num);
    ;
    for (--num; 0 <= num; num--) {
        ++opt;
        def = __builtin_strdup(va_arg(args, const char *));

        r = __builtin_strcmp(arg, def);
        __builtin_free((void *)def);
        def = NULL;

        if (0 == r)
            break;
    }
    ;
    va_end(args);
    ;
    if (num == -1)
        return (unsigned int)num;
    else
        return opt;
}
