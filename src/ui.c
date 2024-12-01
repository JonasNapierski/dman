#include "../include/ui.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

UIElement *checkbox(int y, int x, bool is_checked, const char *text,
                    void *data) {

    if (text == NULL) {
        return NULL; // invalid text
    }

    UICheckbox *box = (UICheckbox *)malloc(sizeof(UICheckbox));

    UIElement *elem = (UIElement *)malloc(sizeof(UIElement));

    if (box == NULL || elem == NULL) {
        return NULL; // not enough memory
    }
    box->text = text;
    box->is_checked = is_checked;

    elem->x = x;
    elem->y = y;
    elem->payload.checkbox = *box;
    elem->type = UI_CHECKBOX;
    elem->next = NULL;
    elem->prev = NULL;
    elem->data = data; // maps data

    return elem;
}

void ui_draw_checkbox(WINDOW *win, UICheckbox *box, int x, int y,
                      bool is_highlighted) {
    if (box == NULL || win == NULL) {
        return; // SOMETHIGN WENT WRONG DURING
    }

    move(y, x);
    wprintw(win, "[");

    if (is_highlighted) {
        wattron(win, A_REVERSE);
    }
    wprintw(win, "%c", box->is_checked ? 'x' : ' ');
    wattroff(win, A_REVERSE);
    wprintw(win, "] %s", box->text);
}

void ui_init() {}

void ui_handle() {}

void ui_draw(WINDOW *win, UIElement ui, bool is_highligthed) {
    switch (ui.type) {
    case UI_CHECKBOX:
        UICheckbox *box = &ui.payload.checkbox;

        ui_draw_checkbox(win, box, ui.x, ui.y, is_highligthed);
        break;
    default:
        break;
    }
}
