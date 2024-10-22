#include "../include/ui.h"

#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTAINER_ID_LEN 100
#define MAX_CONTAINER_NAME_LEN 100

typedef struct Container {
    char container_id[MAX_CONTAINER_ID_LEN];
    char container_name[MAX_CONTAINER_NAME_LEN];
    bool is_selected;
} Container;

int main(void) {
    Container c1;
    strcpy(c1.container_id, "2350487");
    strcpy(c1.container_name, "Test Container");
    c1.is_selected = true;

    Container c2;
    strcpy(c2.container_id, "2350687");
    strcpy(c2.container_name, "Prod Con 12");
    c2.is_selected = false;

    Container *containers = malloc(2 * sizeof(Container));

    containers[0] = c1;
    containers[1] = c2;

    int offset = 10;
    UIElement *container_selected;
    UIElement *container_head;
    UIElement *container_checkbox =
        checkbox(0, offset, false, c1.container_name); // start

    // set the head to the first element;
    container_head = container_checkbox;
    container_selected = container_checkbox;
    for (int i = 1; i < offset; i++) {
        UIElement *next = checkbox(i, offset, false, c2.container_name);

        next->prev = container_checkbox;
        container_checkbox->next = next;
        container_checkbox = container_checkbox->next;
    }
    container_checkbox->next = container_head;
    container_head->prev = container_checkbox;

    char ch;
    initscr();

    curs_set(false);
    noecho();
    nocrmode();
    nodelay(stdscr, true);

    // render loop for the ui
    while ((ch = getch()) != 'q') {
        switch (ch) {
        case 'j':
            if (container_selected->next != NULL) {
                container_selected = container_selected->next;
            }

            break;
        case 'k':
            if (container_selected->prev != NULL) {
                container_selected = container_selected->prev;
            }

            break;
        case ' ':
            container_selected->payload.checkbox.is_checked =
                !container_selected->payload.checkbox.is_checked;

            break;
        default:
            break;
        }
        UIElement *curr = container_head;
        bool isRunning = true;

        while (isRunning) {
            ui_draw(stdscr, *curr, container_selected == curr);
            isRunning = curr->next != NULL && curr->next != container_head;
            curr = curr->next;
        }

        refresh();
    }
    nodelay(stdscr, false);

    endwin();
    // end ui
    free(containers);
    containers = NULL;

    return 0;
}
