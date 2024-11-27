
#ifndef DMAN_H
#define DMAN_H

#include <ncurses.h>

typedef enum DmanState {
    INITIALIZED,
    RUNNING
} DmanState_t;

typedef struct {
    DmanState_t state;
    WINDOW *current;
} Dman;

int dman_init(Dman *dman);
int dman_run(Dman *dman);
int dman_delete(Dman *dman);

#endif
