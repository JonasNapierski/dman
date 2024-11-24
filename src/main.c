#include "../include/engine.h"
#include "../include/ui.h"

#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTAINER_ID_LEN 100
#define MAX_CONTAINER_NAME_LEN 100

int main(void) {
    Container *c1 = (Container *)malloc(sizeof(Container));
    request_docker_api(c1);

    Container *containers = malloc(2 * sizeof(Container));

    UIElement *container_selected;
    UIElement *container_head;
    UIElement *container_checkbox = checkbox(0, 10, false, c1->image); // start

    // set the head to the first element;
    container_head = container_checkbox;
    container_selected = container_checkbox;

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
