// !c++
// #pragma once
//-------------------------------------
// (c) 2018 MIT License Marcel Bobolz
// <mailto:ergotamin.source@gmail.com>
//-------------------------------------
#
// [**`daemonize`**](github.com/ergotamin/snippets)
// **`{`** ``` __implementation__ ``` **`}`**
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
//
