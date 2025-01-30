#include "file.h"
#include "../result/result.h"

#include <stdio.h>

Result open_file(File *f) {
    *f->r = read_file(f);
    return *f->r;
}

Result close_file(File *f) {
    fclose(f->file);
    Result r;

    return r;
}

Result read_file(File *f) {
    f->file = fopen(f->name, f->opt);

    if (f->file == NULL) {
        sets(f->r, fail);
        return *f->r;
    }

    sets(f->r, pass);
    return *f->r;
}
