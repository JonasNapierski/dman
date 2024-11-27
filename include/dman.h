
#ifndef DMAN_H
#define DMAN_H

#include <ncurses.h>

typedef enum DmanState {
    INITIALIZED,
    RUNNING,
    STOPPED,
    ERROR,
} DmanState_t;

typedef struct {
    DmanState_t state;
    int debug_mode; // this modes allows for extra logging etc.
    WINDOW *current;
} Dman;

int dman_init(Dman *dman);
int dman_run(Dman *dman);
int dman_delete(Dman *dman);

#endif
