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

typedef struct UIElement{
    UIElementType type;
    UIELementData data;
    int x;
    int y;
    struct UIElement *next;

} UIElement;

UIElement* checkbox(int y, int x, bool is_checked, const char *text);

void ui_draw(WINDOW *win, UIElement ui, bool is_highligthed);

#endif
