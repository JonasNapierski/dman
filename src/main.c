#include "../include/ui.h"

#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
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

    UIElement *selected;

    UIElement *head = checkbox(0, 10, false, c1.container_name); // start
    UIElement *check2 = checkbox(1, 10, true, c1.container_name);

    head->next = check2; // create an another
    selected = head;

    char ch;
    initscr();

    curs_set(false);
    noecho();
    nocrmode();
    nodelay(stdscr, true);

    // render loop for the ui
    while ((ch = getch()) != 'q') {
        UIElement *curr = head;
        bool isRunning = true;

        while (isRunning) {
            ui_draw(stdscr, *curr, selected == curr);
            isRunning = curr->next != NULL;
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
