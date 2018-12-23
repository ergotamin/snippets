// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin.source@gmail.com>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char **argv)
{
    argc--;
    argv++;
    if (argc) {
        FILE *f = fopen(*argv, "r");
        char buf[1024];
        void *ret = NULL;
        char *tr = 0;
        fprintf(stdout, "static std::Array<std::string,/*LENGTH*/>%s = {\n", *argv);
        do {
            memset(buf, 0, strlen(buf));
            ret = fgets(buf, 1024, f);
            if (NULL != ret) {
                tr = strrchr(buf, '\n');
                *tr = 0;
                fprintf(stdout, "\t\"%s\", \\\n", buf);
            } else {
                fprintf(stdout, "\n};\n// end-of-file.");
                fflush(f);
                fclose(f);
                return EXIT_SUCCESS;
            }
        } while (1);
    } else {
        return EXIT_FAILURE;
    }
}
