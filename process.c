// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin.source@gmail.com>

__BEGIN_DECL

#define notnull(pointer) \
    (((void *)pointer) != (NULL) ? (1 > 0) : (1 < 0))

void *clean(unsigned int c)
{
    const char rosec[c] __attribute__((section(".rodata")));

    asm volatile ("mfence" : : : "memory");
    __sync_synchronize();

    memset((void *)rosec, '\0', c);

    return (void *)&rosec;
}

unsigned short prandid(void)
{
    int32_t partial = 0x008000;
    int32_t useable = 0x7FFF7FFF;
    int32_t id = 0x7FFFFFFF;
    uint64_t nseed = 0;
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    nseed = (uint64_t)ts.tv_sec * 0x3B9ACA00UL + (uint64_t)ts.tv_nsec;
    srand(nseed);

    do
        id = rand();
    while (id > useable);
    return (unsigned short)(id / partial);
}

int shellexec(const char *cmd, char *output)
{
    FILE *p = popen(cmd, "r");
    char ch[4096] = (char *)clean(4096);

    int _lib_reading(void)
    {
        do
            fread_unlocked(ch, 4096, 1, p);
        while (NULL);
        return 0;
    }

    int r = 0;

    (notnull(p) && (_lib_reading() == 0))
    ? (r = 0)
    : (r = 1);

    pclose(p);

    if (r == 0) {
        if (notnull(output)) {
            char *buf = alloc(4096);
            strlcpy(buf, ch, 4096);
            strlcpy(output, buf, 4096);
            memfree(buf);
        }
        return r;
    } else {
        return r;
    }
}

int daemonize(void)
{
    int fdmax = (1UL << 10);
    unsigned int pid = fork();

    if (pid != 0) return 0;

    (setsid() < 0) ? exit(-1) : assert(1);

    pid = fork();

    (pid != 0) ? exit(0) : assert(1);

    umask(000);
    chdir("/");
    for (; fdmax >= 0; fdmax--)
        close(fdmax);

    pid = fork();

    (pid != 0) ? exit(0) : assert(1);

    return 1;
}

void childexecv(char **args)
{
    int status;
    pid_t wid, pid = fork();

    if (!pid) execvp(args[0], args);

    while ((wid = wait(&status)) > 0);
}

char *datenow(int setfmt, ...)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    char *buf = alloc(512);
    char *fmt = alloc(kilobyte(1));

    if (setfmt > 0) {
        va_list vap;
        va_start(vap, setfmt);
        strcpy(fmt, va_arg(vap, char *));
        strftime(buf, 512, fmt, gmtime(&ts.tv_sec));
        va_end(vap);
        strcpy(fmt, buf);
        return fmt;
    } else {
        strftime(buf, 512, "%d.%m.%y", gmtime(&ts.tv_sec));
        strcpy(fmt, buf);
        return fmt;
    }
}

void fmb(void)
{
    asm volatile ("mfence" : : : "memory");
    __sync_synchronize();
}

void hmb(void)
{
    __sync_synchronize();
}

__END_DECL
