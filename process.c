// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin@e-p-s.org>

#include "cstd.h"

__BEGIN_CSTD

int shellexec(const char *cmd, char *output)
{
    FILE *p = popen(cmd, "r");
    char ch[kilobyte(4)] = clean();

    int _lib_reading(void)
    {
        do
            fread_unlocked(ch, kilobyte(4), 1, p);
        while (NULL);
        return VALID;
    }

    int r = SUCCESS;

    (notnull(p) && (_lib_reading() == VALID))
    ? (r = SUCCESS)
    : (r = FAILURE);

    pclose(p);

    if (r == SUCCESS) {
        if (notnull(output)) {
            char *buf = alloc(kilobyte(4));
            strlcpy(buf, ch, kilobyte(4));
            strlcpy(output, buf, kilobyte(4));
            memfree(buf);
        }
        return r;
    } else {
        return r;
    }
}

int daemonize(void)
{
    int fdmax = kilobyte(1);
    pid_t pid = fork();

    if (pid != 0) return SUCCESS;

    (setsid() < 0) ? exit(ERROR) : assert(1);

    pid = fork();

    (pid != 0) ? exit(SUCCESS) : assert(1);

    umask(000);
    chdir("/");
    for (; fdmax >= 0; fdmax--)
        close(fdmax);

    pid = fork();

    (pid != 0) ? exit(SUCCESS) : assert(1);

    return TRUE;
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

__END_CSTD
