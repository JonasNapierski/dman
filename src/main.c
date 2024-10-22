#include "../include/ui.h"

#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Container {
    char container_id[100];
    char container_name[100];
    bool is_selected;
} Container;

int main(void) {
    Container c1;
    strcpy(c1.container_id, "2350487");
    strcpy(c1.container_name, "Test Container");
    c1.is_selected = true;

    Container c2;
    strcpy(c2.container_id, "2350487");
    strcpy(c2.container_name, "Prod Con 12");
    c2.is_selected = false;

    Container *containers = malloc(2 * sizeof(Container));

    containers[0] = c1;
    containers[1] = c2;

    free(containers);
    containers = NULL;

    UICheckbox *check = checkbox(false, false, c1.container_name);
    char ch;
    initscr();

    noecho();
    nodelay(stdscr, true);
    while ((ch = getch()) != 'q') {
        checkbox_draw(check, 10, 8);
        checkbox_draw(check, 10, 9);
        checkbox_draw(check, 10, 10);
        checkbox_draw(check, 10, 11);
        move(8, 11);

        if (ch == 'x') {
            check->selected = !check->selected;
        }

        if (ch == 'j') {
            
            move(newY, 11);
        }

        if (ch == 'k') {
            int newY = getcury(stdscr) - 1;
            move(newY, 11);
        }

        refresh();
    }
    nodelay(stdscr, false);

    endwin();
    return 0;
}
