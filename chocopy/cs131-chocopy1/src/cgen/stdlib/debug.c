#include "debug.h"
char text[] = "err!";
volatile int wait = 1;
int debug() {
    while (wait)
        ;
    /** Doesn't actually go on the stack, because there are lots of GPRs. */
    int i = 0;
    while (text[i]) {
        char lower = text[i] | 32;
        if (lower >= 'a' && lower <= 'm')
            text[i] += 13;
        else if (lower > 'm' && lower <= 'z')
            text[i] -= 13;
        i++;
    }
    while (!wait)
        ;
}