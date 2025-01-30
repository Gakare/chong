#include "shader.h"
#include <stdio.h>
#define BUFFSIZE 1 << 9

/**
 * @param const char* vertex shader file (vsf)
 * @param const char* fragment shader file (fsf)
 * @return void */
void set_shad(const char *vsf, const char *fsf) {
    char vsc[BUFFSIZE];
    char fsc[BUFFSIZE];
    unsigned int vs;
    unsigned int fs;

    FILE *file = fopen(vsf, (char *)'r');

    if (file == NULL) {
        printf("Failed to open file for vertex shader");
        return;
    }

    for (unsigned int i = 0; i < BUFFSIZE; ++i) {
        vsc[i] = (char)fgetc(file);
    }

    fclose(file);

    check_shad(&vs, "VERTEX");

    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const char *const *)vsc, NULL);

    file = fopen(fsf, "r");

    if (file == NULL) {
        printf("Failed to open file for fragment shader\n");
        return;
    }

    for (unsigned int i = 0; i < BUFFSIZE; ++i) {
        fsc[i] = (char)fgetc(file);
    }

    fclose(file);

    check_shad(&fs, "FRAGMENT");

    unsigned int sp;
    sp = glCreateProgram();
    glAttachShader(sp, vs);
    glAttachShader(sp, fs);
    glLinkProgram(sp);

    check_shad(&sp, "PROGRAM");
}

void check_shad(unsigned int *shader, const char *vfp) {
    int success;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[BUFFSIZE];
        glGetShaderInfoLog(*shader, BUFFSIZE, NULL, log);
        printf("ERROR::SHADER::%s::COMPILATION_FAILED\n%s", vfp, log);
    }
}

void set_i(const char *, int);
void set_f(const char *, float);
const void get_ID(struct Shader);
void use();
