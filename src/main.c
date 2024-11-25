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
    Container c1;
    int container_amount = request_docker_api(&c1);

    UIElement *container_selected;
    UIElement *container_head;
    UIElement *container_checkbox = checkbox(0, 10, false, c1.image, &c1);

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
        case '\n':
            Container cont = *(Container *)container_selected->data;
            wprintw(stdscr, "State %s", cont.state);
            wprintw(stdscr, "Status %s", cont.state);
            wprintw(stdscr, "Created %f", cont.created);

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

    return 0;
}
