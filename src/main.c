#include "../include/ui.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Container {
    char container_id[100];
    char container_name[100];
    bool is_selected;
} Container;

void s(Container *containers, int container_amount) {
    for (int i = 0; i < container_amount; i++) {
        char status = ' ';

        if (containers[i].is_selected) {
            status = 'x';
        }
        printf("%d [%c] %s\n", i, status, containers[i].container_name);
    }
}

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

    s(containers, 2);

    free(containers);
    return 0;
}
