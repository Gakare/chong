#ifndef __MFILE__
#define __MFILE__

#include "../result/result.h"
#include <stdio.h>

typedef struct {
    FILE *file;
    char *name;
    char *opt;
    Result *r;
} File;

/* Public: */
Result open_file(File *);
Result close_file(File *);

Result read_file(File *);

#endif
