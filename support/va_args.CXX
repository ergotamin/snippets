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
