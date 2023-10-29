#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/file.h"
#include "sys/stat.h"
#include "errno.h"
#include "time.h"
#include "dirent.h"
#include <string.h>
#include <limits.h>
#include <sys/param.h>

enum
{
    SUF_LEN = sizeof(".exe")
};

void
f_cmp(int *res, char *name)
{
    int len = strlen(name);
    if (len >= SUF_LEN && (!strcmp(".exe", name + len - SUF_LEN))) {
        (*res) += 1;
    }
    return;
}

int
main(int argc, char **argv)
{
    DIR *dd;
    struct dirent *sd;

    dd = opendir(argv[1]);

    int res = 0;

    while ((sd = readdir(dd))) {
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s/%s", argv[1], sd->d_name);
        struct stat s;
        if (stat(path, &s) < 0) {
            continue;
        }

        if (S_ISREG(s.st_mode) && !access(path, X_OK)) {
            f_cmp(&res, path);
        }
    }

    printf("%d\n", res);

    return 0;
}