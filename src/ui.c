#include "../include/ui.h"
#include <ncurses.h>
#include <stdlib.h>

Checkbox_t *checkbox(bool isHovered, bool isChecked, const char *text) {

    if (text == NULL) {
        return NULL; // invalid text
    }

    Checkbox_t *box = (Checkbox_t *)malloc(sizeof(Checkbox_t));

    if (box == NULL) {
        return NULL; // not enough memory
    }

    Checkbox_t box_ = {
        text,
        isHovered,
        isChecked,
    };

    printw("[%c] %s\n",  ' ', text);

    box = &box_;
    return box;
}
