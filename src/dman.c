#include "../include/dman.h"
#include <ncurses.h>

int dman_init(Dman *dman) {
    dman->state = INITIALIZED;
    dman->current = stdscr;

    return 0;
}

int dman_run(Dman *dman) {
    dman->state = RUNNING;


    while (dman->state == RUNNING) {
        // do compute
    }

    return 0;
}
