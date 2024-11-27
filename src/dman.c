#include "../include/dman.h"
#include <ncurses.h>
#include <stdio.h>

int dman_init(Dman *dman) {
    dman->current = stdscr;
    dman->debug_mode = 1; // debug mode is active
    // loading of colorscheme settings etc.
    // $XDG_HOME_DIR/dman.conf or /home/<user>/.config/dman/dman.conf
    // toggle features.

    dman->state = INITIALIZED;
    return 0;
}

int dman_run(Dman *dman) {
    if (dman->state != INITIALIZED) {
        printf("Could not run dman, because initialization did not finished.");
        return -1;
    }

    dman->state = RUNNING;

    while (dman->state == RUNNING) {
        // do compute
    }

    return 0;
}
