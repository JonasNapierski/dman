#ifndef UI_H
#define UI_H
#include <ncurses.h>
#include <stdbool.h>

// This struct exists for managing state of checkbox
typedef struct {
    const char *text;
    bool is_checked;
    void (*action)();

} UICheckbox;

typedef struct {
    const char *text;
    void (*action)();

} UIButton;

typedef enum {
    UI_CHECKBOX,
    UI_BUTTON,
} UIElementType;

typedef union {
    UICheckbox checkbox;
    UIButton button;
} UIELementData;

typedef struct UIElement {
    WINDOW *win;
    UIElementType type;
    UIELementData payload;
    int x;
    int y;
    struct UIElement *next;
    struct UIElement *prev;
    void *data;
} UIElement;

UIElement *checkbox(int y, int x, bool is_checked, const char *text,
                    void *data);

void ui_draw(WINDOW *win, UIElement ui, bool is_highligthed);

#endif
