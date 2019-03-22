//-------------------------------------
// (c) 2018 MIT License Marcel Bobolz
// <mailto:ergotamin.source@gmail.com>
//-------------------------------------
#if !defined(_TRAP_SOURCE)
#error "please sign below the paper..."
#endif

extern const char *__progname;
extern long __syscall(long _nr, ...);
extern int fcntl(int fd, int cmd, ...);
extern int syncfs(int __fd) __attribute__ ((__nothrow__, __leaf__));
extern int *__errno_location(void) __attribute__ ((__nothrow__, __leaf__, __const__));

// *`cpu-architecture dependent syscall execution`*
__asm__  (
    R"(
    .text
    .globl __syscall;
    .type __syscall,@function;
    .align 16;
__syscall:
    .cfi_startproc;
        movq %rdi,    %rax;     # store syscall number in rax
        movq %rsi,    %rdi;     # store argv[1] at argv[0]
        movq %rdx,    %rsi;     # store argv[2] at argv[1]
        movq %rcx,    %rdx;     # store argv[3] at argv[2]
        movq %r8,     %r10;     # store argv[4] at argv[3]
        movq %r9,     %r8;      # store argv[5] at argv[4]
        movq 8(%rsp), %r9;      # get argv[6] from stackpointer
        syscall	                # execute the syscall
        cmpq $-4095,  %rax;     # check for error
        jae	 0f	                # on error jump to error handler
        ret;                    # return to caller

0:      movq errno@GOTTPOFF(%rip), %rcx;
        neg %eax;
        movl %eax, %fs:0(%rcx);
        ;
        or $-1, %rax;
        ret;
    .cfi_endproc;
    .size __syscall,.-__syscall;

# := eof
)");

#define GETFILE            (0X003)
#define SETFILE            (0X004)
#define NONBLOCK           (0X800)
#define READnonblock       (0X000 | NONBLOCK)
#define WRITEnonblock      (0X001 | NONBLOCK)

// *`__builtin_open(const char *fpath, int flags, umode_t mode)`*
#define __builtin_open(fpath, oflags, ...) \
    __syscall(2L, (const char *)fpath, (unsigned int)oflags, ## __VA_ARGS__)
// *`__builtin_close(unsigned int fd)`*
#define __builtin_close(fd) \
    __syscall(3L, (unsigned int)fd)





// ```
// wrapper function for
// ``` [*`sys_syncfs`*](syscall[306])
static unsigned int sync_filesystem(char const *_path)
{
    //
    int _fd = __builtin_open(_path, READnonblock);
    unsigned int _return = 0U;

    if (0 > _fd) {
        //
        int _errno = *(__errno_location());
        _fd = __builtin_open(_path, WRITEnonblock);

        if (_fd < 0) {
            _return = __LINE__;
            __builtin_printf(" error opening: %s  [%i]\n",
                             _path, _errno);
            goto close_exit;
        }
    }

    //
    int _flags = fcntl(_fd, GETFILE);

    if ((-1 == _flags)
        || (0 > fcntl(_fd, SETFILE, _flags & ~NONBLOCK))) {
        //
        _return = __LINE__;
        int _errno = *(__errno_location());
        __builtin_printf(" could not reset `NONBLOCK` mode [%i]\n",
                         _errno);
        goto close_exit;
    }

    //
    int _status = syncfs(_fd);

    if (0 > _status) {
        _return = __LINE__;
        int _errno = *(__errno_location());
        __builtin_printf(" error syncing %s  [%i]\n",
                         _path, _errno);
        goto close_exit;
    }

close_exit:
    if (0 > __builtin_close(_fd)) {
        _return = __LINE__;
        int _errno = *(__errno_location());
        __builtin_printf(" failed to close %s [%i]\n",
                         _path, _errno);
    }
    return _return;
}

// *`entry point`*
int main(int argc, char **argv)
{
    --argc;
    if (argc > 0) {
        int n;
        unsigned int r1 = 0, r2 = 0;
        for (n = 1; argc > 0; --argc, n++) {
            if (0 < (r1 += sync_filesystem(*(argv + n)))) {
                r2 = (r1 - r2);
                __builtin_printf("( %s at line %i )\n",
                                 __FILE__, r2 - 1);
            }
        }
        return r1;
    } else {
        __builtin_printf("Usage:\n%s %s ( %s )\n\t%s\n",
                         __progname,
                         "PATH1 PATH2 ...",
                         "non-network locations",
                         "synchronizes filesystems.");
    }
    return 1;
}
