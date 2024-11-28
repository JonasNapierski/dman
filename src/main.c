#include "../include/engine.h"
#include "../include/ui.h"

#include <complex.h>
#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTAINER_ID_LEN 100
#define MAX_CONTAINER_NAME_LEN 100

int main(void) {
    int x = 4;
    Container **c1 = (Container **)malloc(x * sizeof(Container));

    for (int i = 0; i < x; i++) {
        Container *c0 = (Container *)malloc(sizeof(Container));
        c0->created = -1;
        c0->id = NULL;
        c0->image = NULL;
        c0->image_id = NULL;
        c0->names = NULL;
        c0->state = NULL;
        c0->status = NULL;

        c1[i] = c0;
    }

    int container_amount = engine_get_containers(c1, 4);

    UIElement *container_selected;
    UIElement *container_head;
    UIElement *container_checkbox = checkbox(0, 10, false, c1[0]->image, &c1);

    // set the head to the first element;
    container_head = container_checkbox;
    container_selected = container_checkbox;

    char ch;
    initscr();
    int max_width = getmaxx(stdscr);

    curs_set(false);
    noecho();
    nocrmode();
    nodelay(stdscr, true);
    bool is_active_infobox = false;
    WINDOW *infobox;

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
            is_active_infobox = is_active_infobox == false;

            if (is_active_infobox) {
                Container cont = *(Container *)container_selected->data;
                infobox = newwin(40, max_width - 20, 10, 10);
                refresh();
                box(infobox, 0, 0);
                mvwprintw(infobox, 1, 1, "State %s", cont.state);
                mvwprintw(infobox, 2, 1, "Status %s", cont.state);
                mvwprintw(infobox, 3, 1, "Created %f", cont.created);
                wrefresh(infobox);

            } else {
                werase(infobox);
                wrefresh(infobox);
            }

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
    delwin(infobox);
    endwin();

    return 0;
}
