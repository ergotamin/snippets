// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin@e-p-s.org>
/*!
 *
 *<style type="text/css">
 *table, th, td
 *{
 * border: 1px dashed grey;
 * border-collapse:collapse;
 * width: 100%;
 *}
 *</style>
 *
 * ## _Project:_ `libcstd`
 *
 * ##### `a (private) static C-library`
 *
 * ----
 *
 * <em>
 * This is the libcstd-library-project, which provides
 * a static library that contains functions and definitions
 * used in my projects and/or external builds.
 *
 * It is more or less required to build my projects successful
 * without complaints about missing dependencies. But I am aware
 * of the fact that not everybody is willing to build this library,
 * so I am working on the available projects to be independant of
 * a copy of this. So you are free to use this compilation or not,
 * and as far i can see there is no loss in simply including it to
 * your private collection.
 *
 * [see the license](/LICENSE.md).
 * </em>
 *
 * ----
 *
 * ### Table of Contents
 *
 * The following functions are available:
 * - [basefilename](#basefilename)
 * - [datenow](#datenow)
 * - [file2str](#file2str)
 * - [fullpath](#fullpath)
 * - [int2hex](#int2hex)
 * - [int2str](#int2str)
 * - [md5_of_file](#md5_of_file)
 * - [pathname](#pathname)
 * - [pwd](#pwd)
 * - [strcatarray](#strcatarray)
 * - [strtrim](#strtrim)
 * - [strbuild](#strbuild)
 * - [daemonize](#daemonize)
 * - [filecopy](#filecopy)
 * - [filetest](#filetest)
 * - [getstring](#getstring)
 * - [listdir](#listdir)
 * - [mkdtmpfs](#mkdtmpfs)
 * - [remover](#remover)
 * - [shellexec](#shellexec)
 * - [strarraylen](#strarraylen)
 * - [strswap](#strswap)
 * - [str2file](#str2file)
 * - [sysspawn](#sysspawn)
 * - [randombytes](#randombytes)
 * - [sizeoffile](#sizeoffile)
 * - [strtoreplace](#strtoreplace)
 * - [writetoatoff](#writetoatoff)
 * - [strlcat](#strlcat)
 * - [strlcpy](#strlcpy)
 * - [chk](#chk)
 * - [alloc](#alloc)
 * - [nalloc](#nalloc)
 * - [nop](#nop)
 * - [b2b](#b2b)
 * - [childexecv](#childexecv)
 * - [clr](#clr)
 * - [fmb](#fmb)
 * - [hmb](#hmb)
 * - [memfree](#memfree)
 * - [proginfo](#proginfo)
 * - [set](#set)
 * - [tgl](#tgl)
 *
 * to be continued ...
 */

#if !defined(CSTD_H)

#define CSTD_H __FILE__

#if !defined(__linux__) || !defined(__GNUC__)
#error "Requires Linux OS and a 'GNU-Extension' capable C compiler"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <wait.h>
#include <iconv.h>
#include <sys/capability.h>
#include <sys/inotify.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/random.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/capability.h>
#include <linux/mman.h>
#include <linux/types.h>
#include <linux/limits.h>
#include <asm-generic/types.h>
#include <assert.h>
#include <crypt.h>
#include <ctype.h>
#include <dirent.h>
#include <ftw.h>
#include <glob.h>
#include <libgen.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <pthread.h>
#include <wchar.h>
#include <wordexp.h>

#if defined(__cplusplus)
#define __BEGIN_CSTD    extern "C" {
#define __END_CSTD  }
#else
#define __BEGIN_CSTD
#define __END_CSTD
#endif

__BEGIN_CSTD

/* author, e-mail, license for proginfo()-function */

#define AUTHORNAME "Marcel Bobolz"
#define AUTHORMAIL "<ergotamin@e-p-s.org>"
#define LICENSE    "MIT License (C) 2018 All rights reserved."

/* function/variable attribute-macros */

#define __extern__      __attribute__((extern, externally_visible))
#define __inline__      __attribute__((inline, gnu_inline, always_inline))
#define __not_used__    __attribute__((unused))
#define __constructor__ __attribute__((constructor))
#define __destructor__  __attribute__((destructor))

/* redefinitions (glib-conform)*/

#undef NULL
#undef TRUE
#undef FALSE

#define NULL          ((void *)0L)
#define FALSE         (1 < 0)
#define TRUE          (1 > 0)

/* bytesizes and 64Bit-variable sizes 64Bit */

#define megabyte(n)   ((1UL << 20) * n)
#define kilobyte(n)   ((1UL << 10) * n)
#define sizeofchar    (1UL << 0)
#define sizeofint     (1UL << 2)
#define sizeoflong    (1UL << 3)
#define sizeofptr     (1UL << 3)

/* used to check if a process is alive with *kill()* */

#define SIGTEST       (0UL)

/* basic return values */

enum returnvalue {
    VALID		= 0,
    SUCCESS		= 0,
    FAILURE		= 1,
    INVALID		= 2,
    EXCEPTION	= 3,
    ERROR		= -1,
    UNDEF		= -2,
    PFAULT		= -3,
};

/* 'tput' color-escapes */

#define fg(r, g, b)   "\x1b[38:2:"r ":"g ":"b "m"
#define bg(r, g, b)   "\x1b[48:2:"r ":"g ":"b "m"
#define sgr()         "\x1b(B\x1b[m"
#define bold()        "\x1b[1m"
#define smul()        "\x1b[4m"
#define sitm()        "\x1b[3m"
#define el1()         "\x1b[1K"
#define up1()         "\x1b[A"

/* commandline print macros */

#define cprint(format, args ...) \
    (fprintf(stdout, format, ## args))

#define cerror(string, ...)         \
    fprintf(stderr,                 \
            fg("255", "255", "255") \
            bg("255", "0", "40")    \
            "\t>_ ERROR :"          \
            sgr()                   \
            fg("255", "0", "0")     \
            "\t"string ""           \
            sgr()                   \
            "\n", ## __VA_ARGS__)

/* if-case assertion */

#define notnull(pointer) \
    (((void *)pointer) != (NULL) ? (TRUE) : (FALSE))

/* 'clean' array initializer */

#define clean() { }

/* full-array-iteration macro */

#define foreach(apointer) \
    for (; (void *) *apointer != NULL; apointer ++)

/*!----
 *
 * ##### basefilename
 *
 *```c
 * char *basefilename(const char *fname);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *fname | `in` | *A path of a file.*
 *
 *<center>
 *
 *> Is used to determine the basename of
 *> a /path/to/file without the suffix
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string which represents the basename.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *basefilename(const char *fpath);

/*!----
 *
 * ##### datenow
 *
 *```c
 * char *datenow(int setfmt, ...);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **int** setfmt | `in` | *Whether varargs describe a format or not*
 * **char** *... | `in` | *A 'mkstrftime' format-string or NULL*
 *
 *<center>
 *
 *> Gets the current date in default format %d.%m.%y,
 *> or if *setfmt* >= 1 in the format submitted.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string which represents the current date.</dd>
 *</dl>
 *</center>
 */
char *datenow(int setfmt, ...);

/*!----
 *
 * ##### file2str
 *
 *```c
 * char *file2str(const char *fpath);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *fpath | `in` | *A path of a file.*
 *
 *<center>
 *
 *> Reads the file in *fpath* to a newly allocated
 *> buffer, and returns the contents as a string.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string which represents the file-contents.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *file2str(const char *fpath);

/*!----
 *
 * ##### fullpath
 *
 *```c
 * char *fullpath(const char *path);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *path | `in` | *A path on filesystem.*
 *
 *<center>
 *
 *> Locates the file/directory behind path,
 *> and returns the absolute file-system path.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string which represents absolute path.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *fullpath(const char *path);

/*!----
 *
 * ##### int2hex
 *
 *```c
 * char *int2hex(unsigned int i);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **int** *i | `in` | *A positive integer value.*
 *
 *<center>
 *
 *> Uses sprintf() internally to transform an
 *> integer value to its hexadecimal compliant.
 *> (printf format: %X6.6 (!))
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string which represents a hex value.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *int2hex(unsigned int i);

/*!----
 *
 * ##### int2str
 *
 *```c
 * char *int2str(int i);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **int** *i | `in` | *An integer value.*
 *
 *<center>
 *
 *> Uses sprintf() internally to transform an
 *> integer value to its string compliant.
 *> ( overloaded 512Byte allocation(!) )
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string which represents an integer-value.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *int2str(int i);

/*!----
 *
 * ##### md5_of_file
 *
 *```c
 * char *md5_of_file(const char *fname);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *fname | `in` | *A path of a file.*
 *
 *<center>
 *
 *> Uses the OpenSSL-algrorithm to form the
 *> md5-checksum of a file named *fname*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string which represents the md5-checksum.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *md5_of_file(const char *fname);

/*!----
 *
 * ##### pathname
 *
 *```c
 * char *pathname(const char *path);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *path | `in` | *A path of a file/directory.*
 *
 *<center>
 *
 *> Truncates the top file/directory part
 *> of *path* and returns what is left.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string which should represent the parent path.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *pathname(const char *path);

/*!----
 *
 * ##### pwd
 *
 *```c
 * char *pwd(short cout);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **short** cout | `in` | *TRUE or FALSE, whether to print to console.*
 *
 *<center>
 *
 *> Retrieves the working directory of the
 *> current process and optionally prints to console.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string which should represents the current path.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *pwd(short cout);

/*!----
 *
 * ##### strcatarray
 *
 *```c
 * char *strcatarray(char **strarr,int count);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** **strarr | `in` | *An array of strings*
 * **int** count | `in` | *Count of objects to concatenate.*
 *
 *<center>
 *
 *> Concatenates *count* number of strings in
 *> an array *strarr* and returns the resulting string.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string with the arrays objects as content.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *strcatarray(char **strarr, int count);

/*!----
 *
 * ##### strtrim
 *
 *```c
 * char *strtrim(char *str,char ch);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *str | `in` | *A string to trim.*
 * **char** ch | `in` | *The character at which to trim.*
 *
 *<center>
 *
 *> Trims the string *str* at the last occurence
 *> of *ch* left-to-right.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A trimmed string.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
char *strtrim(char *str, char ch);

/*!----
 *
 * ##### strbuild
 *
 *```c
 * char *strbuild(char *format,...);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *format | `in` | *A printf-format string.*
 * **var** ... | `in` | *The variables to get stringified.*
 *
 *<center>
 *
 *> Builds a string from a printf-format string
 *> containing all the parameters submitted.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A string.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
const char *strbuild(char *format, ...);

/*!----
 *
 * ##### daemonize
 *
 *```c
 * int daemonize(void);
 *```
 *
 *<center>
 *
 *> Deattaches a child-process completly from
 *> its parent by closing all file-descriptors
 *> and forking.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>TRUE for the forked child.</dd>
 * <dd>FALSE for the calling parent.</dd>
 *</dl>
 *</center>
 */
int daemonize(void);

/*!----
 *
 * ##### filecopy
 *
 *```c
 * int filecopy(char *in, char *out);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *in | `in` | *A path to a file.*
 * **char** *out | `out` | *A path to a new file.*
 *
 *<center>
 *
 *> Copies a file from *in* to *out*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>SUCCESS on success.</dd>
 * <dd>FAILURE on failure.</dd>
 *</dl>
 *</center>
 */
int filecopy(char *in, char *out);

/*!----
 *
 * ##### filetest
 *
 *```c
 * char *filetest(const char *fname);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *fname | `in` | *A path to test for existence.*
 *
 *<center>
 *
 *> Tests for file-existence.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>SUCCESS on success.</dd>
 * <dd>FAILURE on failure.</dd>
 *</dl>
 *</center>
 */
int filetest(const char *fname);

/*!----
 *
 * ##### getstring
 *
 *```c
 * int getstring(char *buf);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *buf | `in/out` | *A string-pointer.*
 *
 *<center>
 *
 *> Reads a string from stdin and returns
 *> it in *buf*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>count of characters on success.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
int getstring(char *buf);

/*!----
 *
 * ##### listdir
 *
 *```c
 * int listdir(char *dir,char **retbuf);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *dir | `in` | *A path to a directory.*
 * **char** **retbuf | `in/out` | *The returned array.*
 *
 *<center>
 *
 *> Iterates recursive through the given directory
 *> and saves each file/directory in *retbuf*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>SUCCESS on success.</dd>
 * <dd>FAILURE on failure.</dd>
 *</dl>
 *</center>
 */
int listdir(char *dir, char **retbuf);

/*!----
 *
 * ##### mkdtmpfs
 *
 *```c
 * int mkdtmpfs(char *mntpath);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *mntpath | `in` | *The location to mount at.*
 *
 *<center>
 *
 *> Mounts a temporary filesystem at *mntpath*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>SUCCESS on success.</dd>
 * <dd>FAILURE on failure.</dd>
 *</dl>
 *</center>
 */
int mkdtmpfs(char *mntpath);

/*!----
 *
 * ##### remover
 *
 *```c
 * int remover(char *fpath);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *fpath | `in` | *The file/directory to remove.*
 *
 *<center>
 *
 *> Recursive removal of a directory or
 *> simple removal of a given file at *fpath*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>SUCCESS on success.</dd>
 * <dd>FAILURE on failure.</dd>
 *</dl>
 *</center>
 */
int remover(char *fpath);

/*!----
 *
 * ##### shellexec
 *
 *```c
 * int shellexec(const char *cmd, char *output);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *cmd | `in` | *A command string to execute.*
 * **char** *output | `out` | *Returning stdout-buffer or NULL.*
 *
 *<center>
 *
 *> Executes *cmd* in a shell and returns the output
 *> with *output* if non-NULL was submitted.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>SUCCESS on success.</dd>
 * <dd>FAILURE on failure.</dd>
 *</dl>
 *</center>
 */
int shellexec(const char *cmd, char *output);

/*!----
 *
 * ##### strarraylen
 *
 *```c
 * int strarraylen(char **strarray);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** **strarray | `in` | *An array of strings.*
 *
 *<center>
 *
 *> Used to determine the count of strings in *strarray*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>count of object on success.</dd>
 * <dd>zero on failure.</dd>
 *</dl>
 *</center>
 */
int strarraylen(char **strarray);

/*!----
 *
 * ##### strswap
 *
 *```c
 * int strswap(char *strarray,char *from);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *to | `in/out` | *An allocated string.*
 * **char** *from | `in/out` | *An allocated string.*
 *
 *<center>
 *
 *> Swaps the memory contents *from* to *to*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>SUCCESS on success.</dd>
 * <dd>FAILURE on failure.</dd>
 *</dl>
 *</center>
 */
int strswap(char *to, char *from);

/*!----
 *
 * ##### str2file
 *
 *```c
 * int str2file(const char *fname,char *str);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *fname | `in` | *Name of the target-file.*
 * **char** *str | `in` | *An string for file-contents.*
 *
 *<center>
 *
 *> Writes contents of *str* to a new file *fname*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>SUCCESS on success.</dd>
 * <dd>FAILURE on failure.</dd>
 *</dl>
 *</center>
 */
int str2file(const char *fname, char *str);

/*!----
 *
 * ##### sysspawn
 *
 *```c
 * int sysspawn(const char *fmt, ...);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *fmt | `in` | *A printf-conform format-string.*
 * **var** ... | `in` | *Variadic args for the format-string.*
 *
 *<center>
 *
 *> A wrapper for the system()-function, to
 *> be able to submit variables.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>The exit-code of the system()-function.</dd>
 *</dl>
 *</center>
 */
int sysspawn(const char *fmt, ...);

/*!----
 *
 * ##### randombytes
 *
 *```c
 * long int randombytes(long unsigned int le);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **long unsigned int** le | `in` | *Count of bytes requested.*
 *
 *<center>
 *
 *> Uses the linux /dev/random devices' entropy
 *> to return random-bytes of count *le*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>Random-Bytes on success.</dd>
 * <dd>Zero on failure.</dd>
 *</dl>
 *</center>
 */
long int randombytes(long unsigned int le);

/*!----
 *
 * ##### sizeoffile
 *
 *```c
 * long int sizeoffile(const char *fname);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **const char** *fname | `in` | *Path to a file.*
 *
 *<center>
 *
 *> Retrieves the length in bytes of a given file.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>The size in bytes on success.</dd>
 * <dd>Zero on failure.</dd>
 *</dl>
 *</center>
 */
long int sizeoffile(const char *fname);

/*!----
 *
 * ##### strtoreplace
 *
 *```c
 * long int strtoreplace(char *buf, const char *template, const char *replacer);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *buf | `in/out` | *An allocated string.*
 * **const char** *template | `in` | *The template to look for.*
 * **const char** *replacer | `in` | *The replacement for the template.*
 *
 *<center>
 *
 *> Iterates through a string in *buf* and replaces
 *> any occurence of *template* with *replacer*.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>Count of replacements on success.</dd>
 * <dd>Zero on failure.</dd>
 *</dl>
 *</center>
 */
long int strtoreplace(char *buf, const char *template, const char *replacer);

/*!----
 *
 * ##### writetoatoff
 *
 *```c
 * long int writetoatoff(char *fname, long int offset, char *input);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *fname | `in` | *Path of the target file.*
 * **long int** offset | `in` | *The offset to skip.*
 * **char** *input | `in/out` | *The data to write.*
 *
 *<center>
 *
 *> Skips the *offset* in *fname* and starts writing
 *> the *input* to a given file.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>Count of characters writtten on success.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
long int writetoatoff(char *fname, long int offset, char *input);

/*!----
 *
 * ##### strlcat
 *
 *```c
 * long int strlcat(char *dst, const char *src, long unsigned int sz);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *dst | `in/out` | *The target string to append to.*
 * **const char** *src | `in` | *The source which is to append.*
 * **long unsigned int** sz | `in` | *Maximum lenght of the resulting string.*
 *
 *<center>
 *
 *> Always ZERO-terminated string-concatenation.
 *> Excerpt from BSD-sources.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>Lenght of string on success.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
long unsigned int strlcat(char *dst, const char *src, long unsigned int sz);

/*!----
 *
 * ##### strlcpy
 *
 *```c
 * long int strlcpy(char *dst, const char *src, long unsigned int sz);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *dst | `in/out` | *The target string to append to.*
 * **const char** *src | `in` | *The source which is to append.*
 * **long unsigned int** sz | `in` | *Maximum lenght of the resulting string.*
 *
 *<center>
 *
 *> Always ZERO-terminated string-copy.
 *> Excerpt from BSD-sources.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>Lenght of string on success.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
long unsigned int strlcpy(char *dst, const char *src, long unsigned int sz);

/*!----
 *
 * ##### chk
 *
 *```c
 * unsigned int chk(void *addr32, unsigned int bit);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *addr | `in` | *A 32Bit memory-address.*
 * **unsigned int** bit | `in` | *The bit to check.*
 *
 *<center>
 *
 *> Checks whether a bit in *addr32* is set or not.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>TRUE on success.</dd>
 * <dd>FALSE on failure.</dd>
 *</dl>
 *</center>
 */
unsigned int chk(void *addr32, unsigned int bit);

/*!----
 *
 * ##### alloc
 *
 *```c
 * void *alloc(long unsigned int bytes);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **long unsigned int** bytes | `in` | *Count of bytes to allocate.*
 *
 *<center>
 *
 *> Allocates *bytes* in memory.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A pointer to allocated bytes on success.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
void *alloc(long unsigned int bytes);

/*!----
 *
 * ##### nalloc
 *
 *```c
 * void *nalloc(long unsigned int nblocks, long unsigned int nbytes);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **long unsigned int** nblocks | `in` | *Count of blocks to allocate.*
 * **long unsigned int** nbytes | `in` | *Count of bytes to allocate.*
 *
 *<center>
 *
 *> Allocates *nblocks* of *nbytes* in memory.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>A pointer to allocated bytes on success.</dd>
 * <dd>NULL on failure.</dd>
 *</dl>
 *</center>
 */
void *nalloc(long unsigned int nblocks, long unsigned int nbytes);

/*!----
 *
 * ##### nop
 *
 *```c
 * void *nop(void);
 *```
 *
 *<center>
 *
 *> Does nothing -> No Operation.
 *
 *<dl>
 * <dt>Returns:</dt>
 * <dd>NULL.</dd>
 *</dl>
 *</center>
 */
void *nop(void);

/*!----
 *
 * ##### b2b
 *
 *```c
 * void b2b(void *addr32, unsigned int from, int to);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *addr | `in` | *A 32Bit memory-address.*
 * **unsigned int** from | `in` | *The value to change from.*
 * **int** to | `in` | *The value to change to.*
 *
 *<center>
 *
 *> 'b2b' changes a bit *from* to *to* -value.
 *
 *</center>
 */
void b2b(void *addr32, unsigned int from, int to);

/*!----
 *
 * ##### childexecv
 *
 *```c
 * void *childexecv(char **args);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** **args | `in` | *execl conform array of a command.*
 *
 *<center>
 *
 *> Lets a child-process execute *args* and waits for its exit.
 *> The exit-code of the child-process is not checken(!).
 *
 *</center>
 */
void childexecv(char **args);

/*!----
 *
 * ##### clr
 *
 *```c
 * void clr(void *addr32, unsigned int bit);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *addr | `in` | *A 32Bit memory-address.*
 * **unsigned int** bit | `in` | *The bit to get cleared.*
 *
 *<center>
 *
 *> 'clr' clears a bit at *addr32*.
 *
 *</center>
 */
void clr(void *addr32, unsigned int bit);

/*!----
 *
 * ##### cputc
 *
 *```c
 * void cputc(char ch, FILE *filep);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** ch | `in/out` | *A character to get written.*
 * **FILE** *filep | `in` | *A pointer to an open FILE .*
 *
 *<center>
 *
 *> Unblocked version of the (f)putc()-function.
 *
 *</center>
 */
void cputc(char ch, FILE *filep);

/*!----
 *
 * ##### fmb
 *
 *```c
 * void fmb(void);
 *```
 *
 *<center>
 *
 *> Performs a full-memory barrier (soft- and hardware).
 *
 *</center>
 */
void fmb(void);

/*!----
 *
 * ##### fmb
 *
 *```c
 * void fmb(void);
 *```
 *
 *<center>
 *
 *> Performs a half-memory barrier (software).
 *
 *</center>
 */
void hmb(void);

/*!----
 *
 * ##### memfree
 *
 *```c
 * void memfree(void *ptr);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **void** *ptr | `in` | *A pointer to allocated memory.*
 *
 *<center>
 *
 *> Frees the memory to which *ptr* is pointing to.
 *> Save even if *ptr* was not allocated (!).
 *
 *</center>
 */
void memfree(void *ptr);

/*!----
 *
 * ##### proginfo
 *
 *```c
 * void proginfo(char *desc);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *desc | `in` | *An application-description.*
 *
 *<center>
 *
 *> Prints *desc* with some additional information.
 *> Defined above as AUTHORNAME, AUTHORMAIL, LICENSE.
 *> Feel free to change !
 *
 *</center>
 */
void proginfo(char *desc);

/*!----
 *
 * ##### set
 *
 *```c
 * void set(void *addr32, unsigned int bit);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *addr | `in` | *A 32Bit memory-address.*
 * **unsigned int** bit | `in` | *The bit to get set.*
 *
 *<center>
 *
 *> 'set' sets a bit at *addr32*.
 *
 *</center>
 */
void set(void *addr32, unsigned int bit);

/*!----
 *
 * ##### tgl
 *
 *```c
 * void tgl(void *addr32, unsigned int bit);
 *```
 *
 * parameter | in/out | description
 * ---- |:----:| ----
 * **char** *addr | `in` | *A 32Bit memory-address.*
 * **unsigned int** bit | `in` | *The bit to toggle.*
 *
 *<center>
 *
 *> 'tgl' toggles a bit at *addr32*.
 *
 *</center>
 */
void tgl(void *addr32, unsigned int bit);

__END_CSTD

#endif
