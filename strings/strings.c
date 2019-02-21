// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin.source@gmail.com>

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

long unsigned int strlcpy(char *dst, const char *src, long unsigned int sz)
{
    const char *osrc = src;
    long unsigned int bytes = sz;

    if (bytes != 0)
        while (--bytes != 0)
            if ((*dst++ = *src++) == '\0') break;

    if (bytes == 0) {
        if (sz != 0) *dst = '\0';

        while (*src++);
    }
    return src - osrc - 1;
}

char *strtrim(char *str, char ch)
{
    char *s = strrchr(str, ch);

    if (s) {
        *s = 0;
        return str;
    }
    return s;
}

const char *strbuild(char *format, ...)
{
    char *buff = alloc(kilobyte(4));

    va_list argp;

    va_start(argp, format);
    vsprintf(buff, format, argp);
    va_end(argp);

    return strdup(buff);
}

char *strcatarray(char **strarr, int count)
{
    int fule = 0;
    int sile = 0;
    int nr = 0;

    for (; nr < count; nr++) {
        fule += strlen(strarr[nr]);

        if (strarr[nr][strlen(strarr[nr]) - 1] != '\n') fule++;
    }
    fule++;
    char *fullstr = alloc(fule);
    fullstr[0] = '\0';

    for (nr = 0; nr < count; nr++) {
        strcat(fullstr, " ");
        strcat(fullstr, strarr[nr]);
        sile = strlen(fullstr);

        if (fullstr[sile - 1] != '\n') {
            fullstr[sile] = '\n';
            fullstr[sile] = '\0';
        }
    }
    return fullstr;
}

int strarraylen(char **strarray)
{
    int le = 0;

    for (; (void *)*strarray != NULL; strarray++, le++)
        ;
    return le;
}

int strswap(char *to, char *from)
{
    char savebuf[((strlen(to)) > (strlen(from)) ? (strlen(to) + 1) : (strlen(from) + 1))];
    {
        if (strcpy(savebuf, to) != NULL)
            if (strcpy(to, from) != NULL)
                if (strcpy(from, savebuf) != NULL) return SUCCESS;
    }

    return ERROR;
}

long int strtoreplace(char *buf, const char *template, const char *replacer)
{
    int tlen, rlen;
    char *str, *dstr;
    char *tmpbuf = NULL;

    tmpbuf = alloca(strlen(buf) + 1);

    if (!buf ||       \
        !*buf ||      \
        !template ||  \
        !*template || \
        !replacer) return 0;

    if (tmpbuf == NULL) return 0;
    tlen = strlen(template);
    rlen = strlen(replacer);

    str = buf;
    dstr = tmpbuf;

    while (*str) {
        if (strncmp(str, template, tlen) == 0) {
            strcpy(dstr, replacer);
            str += tlen;
            dstr += rlen;
        } else {
            *dstr++ = *str++;
        }
    }
    *dstr = '\0';

    strcpy(buf, tmpbuf);
    return strlen(buf);
}

int getstring(char *buf)
{
    long unsigned int le = kilobyte(4);
    char str[le];

    (fgets(str, le, stdin) != NULL) ? (le = strlen(str)) : (le = 0);
    ((le > 0) && (str[le - 1] == 10)) ? (str[le - 1] = 0) : (0);
    strlcpy(buf, str, kilobyte(4));
    memset(str, 0, le);
    return str[0];
}

char *int2str(int i)
{
    char str[512];
    char *ret = alloc(512);

    sprintf(str, "%i", i);
    strcpy(ret, str);
    return ret;
}

char *int2hex(unsigned int i)
{
    long unsigned int le = 0;
    long unsigned int hexle = strlen("0xBEDEAD~");
    char *hex = alloc(hexle);
    char *format = "0x%6.6x";

    le = snprintf(hex, hexle, format, i);

    if (le) return hex;
    else return (char *)NULL;
}

__END_DECL
