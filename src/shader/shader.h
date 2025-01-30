#ifndef __MSHADER__
#define __MSHADER__

#include <GLES3/gl3.h>

struct Shader {
    unsigned int ID;
};

void set_shad(const char *, const char *);
void check_shad(unsigned int *, const char *);
void set_i(const char *, int);
void set_f(const char *, float);
const void get_ID(struct Shader);
void use();

#endif
