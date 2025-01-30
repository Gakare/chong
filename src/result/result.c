#include "result.h"

/* Sets the setter in Result and changes the state of Result */
void sets(Result *r, int i) {
    r->Set = i;
    setr(r);
}

/* This should be hidden away */
void setr(Result *r) {
    if (r->Set == 0) {
        r->Ok = 1;
        r->Err = 0;
    } else {
        r->Err = 1;
        r->Ok = 0;
    }
}
