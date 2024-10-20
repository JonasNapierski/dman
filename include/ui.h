#ifndef UI_H
#define UI_H

#include <stdbool.h>

// This struct exists for managing state of checkbox
typedef struct Checkbox_t {
    const char *text;
    bool hovered;
    bool selected;

} Checkbox_t;

Checkbox_t *checkbox(bool isHovered, bool isChecked, const char *text);
int checkbox_check(Checkbox_t chbox, bool isChecked);
int checkbox_hover(Checkbox_t chbox, bool isHovered);

#endif
