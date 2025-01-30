#ifndef __MRESULT__
#define __MRESULT__

/**
 * pass = 0
 * fail = 1 */
typedef enum {
    pass = 0,
    fail = 1,
} set;

typedef struct {
    /* Only thing to modify is setter */
    set Set;

    unsigned int Ok : 1;
    unsigned int Err : 1;
} Result;

void sets(Result *, int);

/* Should be hidden away */
void setr(Result *);

#endif
