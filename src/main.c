#include "../include/engine.h"
#include "../include/ui.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <curl/easy.h>
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

void s(Container *containers, int container_amount) {
    for (int i = 0; i < container_amount; i++) {
        char status = ' ';

        if (containers[i].is_selected) {
            status = 'x';
        }
        printw("%d [%c] %s\n", i, status, containers[i].container_name);
    }
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

// TODO: Move most of this code into engine.c and create a data model for the
// rest of this project to be used.

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

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL,
                         "http://localhost/v1.47/containers/json?all=true");
        curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH,
                         "/var/run/docker.sock");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,
                         stdout);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            return -1;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    char ch;
    initscr();

    noecho();
    while ((ch = getch()) != 'q') {
        s(containers, 2);
        refresh();
    }

    endwin();
    return 0;
}
