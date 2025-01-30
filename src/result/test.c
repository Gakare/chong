#include "result.h"

#include <stdio.h>
#include <stdlib.h>

void test_Ok(Result *r) {
    char *clear = "\x1B[0m";
    char *green = "\x1B[32m";
    char *red = "\x1B[31m";
    if (r->Ok == 0 && r->Err == 1) {
        printf("%sResult Ok: Failed%s\n", red, clear);
        return;
    }
    printf("%sResult Ok: Passed%s\n", green, clear);
}

void test_Err(Result *r) {
    char *clear = "\x1B[0m";
    char *green = "\x1B[32m";
    char *red = "\x1B[31m";
    if (r->Err == 0 && r->Ok == 1) {
        printf("%sResult Err: Failed%s\n", red, clear);
        return;
    }
    printf("%sResult Err: Passed%s\n", green, clear);
}

void test_Set(Result *r, set *s) {
    char *clear = "\x1B[0m";
    char *green = "\x1B[32m";
    char *red = "\x1B[31m";
    if (r->Set != *s) {
        printf("%sResult Setter: Failed%s\n", red, clear);
        return;
    }
    printf("%sResult Setter: Passed%s\n", green, clear);
}

int main() {
    Result r1;
    r1.Set = pass;
    /* Test 1 */
    setr(&r1);
    printf("Testing if setr will set Ok: ");
    test_Ok(&r1);

    Result r2;
    r2.Set = fail;
    setr(&r2);
    printf("Testing if setr will set Err: ");
    test_Err(&r2);

    sets(&r1, 1);
    set s1 = 1;
    printf("Testing if sets will set Fail: ");
    test_Set(&r1, &s1);

    sets(&r2, 0);
    set s2 = 0;
    printf("Testing if sets will set Pass: ");
    test_Set(&r2, &s2);

    exit(0);
}
