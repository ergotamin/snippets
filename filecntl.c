// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin@e-p-s.org>

#include "cstd.h"

__BEGIN_CSTD

char *basefilename(const char *fpath)
{
    char *tr = NULL;
    char *bname = alloc(512);
    char *temp = alloc(strlen(fpath) + 1);

    temp = strdup(fpath);
    bname = basename(temp);
    {
        while ((tr = strrchr(bname, '.')) != NULL)
            *tr = '\0';
    }
    if (*bname)
        return bname;
    return NULL;
}

char *pathname(const char *path)
{
    char n = '/';
    int len = strlen(path);
    char temp[512];
    char *parent = alloc(512);

    strlcpy(temp, path, 512);
    {
        if (temp[len] == n)
            temp[len] = '\0';

        if (strtrim(temp, '/') == VALID)
            strlcpy(parent, temp, 512);
    }
    if (*parent)
        return parent;
    return NULL;
}

char *fullpath(const char *path)
{
    char *abs_path;

    return abs_path = realpath(path, NULL);
}

char *pwd(short cout)
{
    char *cwd = alloc(512);

    getcwd(cwd, 512);

    if (*cwd) {
        if (cout == TRUE)
            cprint("%s\n", cwd);

        return cwd;
    }
    return NULL;
}

void cputc(char ch, FILE *filep)
{
    __putc_unlocked_body(ch, filep);
}

int filetest(const char *fname)
{
    if ((access(fname, F_OK)) != -1)
        return VALID;
    else return INVALID;
}

long int sizeoffile(const char *fname)
{
    long int len;
    FILE *this_file = fopen(fname, "rb");

    if (this_file) {
        fseek(this_file, 0, SEEK_END);
        len = ftell(this_file);
        fclose(this_file);
        return len;
    }
    return 0L;
}

int filecopy(char *in, char *out)
{
    unsigned char buff[kilobyte(4)];
    FILE *src = fopen(in, "rb");
    FILE *dst = fopen(out, "wb+");
    long unsigned int nbyte;

    while ((nbyte = fread((char *)buff, sizeofchar, sizeof(buff), src)) > 0)
        if (fwrite((char *)buff, sizeofchar, nbyte, dst) != nbyte)
            return FAILURE;

    fflush(dst);
    fclose(dst);
    fclose(src);
    return SUCCESS;
}

int str2file(const char *fname, char *str)
{
    FILE *this_file = fopen(fname, "wb+");
    int len = strlen(str);

    if (this_file) {
        while (len--)
            fwrite(str++, sizeofchar, 1, this_file);

        fflush(this_file);
        fclose(this_file);
        return SUCCESS;
    }
    return FAILURE;
}

char *file2str(const char *fpath)
{
    char *str;
    long int len = 0;

    FILE *this_file = fopen(fpath, "rb");

    if (this_file) {
        len = sizeoffile(fpath);
        unsigned char buff[sizeofchar * len + 1];

        fread(buff, 1, len, this_file);
        fclose(this_file);
        str = alloc(len + 1);
        strcpy(str, (const char *)buff);

        memset(buff, 0, len + 1);
        return str;
    }
    return NULL;
}

long int writetoatoff(char *fname, long int offset, char *input)
{
    long int len;
    FILE *this_file = fopen(fname, "rb+");

    if (this_file)
        fseek(this_file, offset, SEEK_SET);
    else return FAILURE;

    if (filetest(input) == VALID) {
        len = sizeoffile(input) + 1;
        char *fstring = file2str(input);

        while ((len != 0) && (*fstring)) {
            cputc(*fstring, this_file);
            fstring++;
            len--;
        }
        fflush(this_file);
    } else {
        len = (strlen(input) + 1);

        while (*input != 0) {
            cputc(*input, this_file);
            input++;
        }
        fflush(this_file);
    }
    fclose(this_file);
    return len;
}

int mkdtmpfs(char *mntpath)
{
    int ret;
    char *mntpoint = mkdtemp(mntpath);
    const char *mntopts = "rw,noatime,nodiratime,nodev,mode=1777";

    if (!mntpoint)
        return FAILURE;

    ret = mount("tmpfs", mntpoint, "tmpfs", 0, mntopts);

    if (!ret)
        return SUCCESS;

    cerror("tmpfs creation failed");
    rmdir(mntpoint);
    return FAILURE;
}



int remover(char *fpath)
{
    int _lib_remover(const char *				path,
                     const struct stat *fstat	__not_used__,
                     int ftype					__not_used__,
                     struct FTW *Ftwb			__not_used__)
    {
        if (path)
            if (remove(path) < 0)
                return SUCCESS;

        return SUCCESS;
    }
    {
        if (remove(fpath))
            if (nftw(fpath, _lib_remover, 24, FTW_PHYS | FTW_DEPTH) < 0)
                return ERROR;
    }
    return SUCCESS;
}

int listdir(char *dir, char **retbuf)
{
    short is_cwd = 0;

    if (strcmp(dir, ".") == VALID) is_cwd = 1;

    int _lib_listdir(const char *				path,
                     const struct stat *fstat	__not_used__,
                     int						ftype,
                     struct FTW *Ftwb			__not_used__)
    {
        if ((ftype == FTW_F) || (ftype == FTW_SL) || (ftype == FTW_D)) {
            *retbuf = alloc(512);
            strlcpy(*retbuf, path, 512);
            if (is_cwd == 1) strtoreplace(*retbuf, "./", "");
            retbuf++;
        }
        return SUCCESS;
    }

    if (nftw(dir, _lib_listdir, 24, FTW_DEPTH | FTW_PHYS) < 0)
        return ERROR;
    return SUCCESS;
}

__END_CSTD
