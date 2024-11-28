// NOTES for v0.1.0
//
// create a structure for the container
// - uuid
// - name
// - status
//
// method stop,start list all container
#include "../include/engine.h"

#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char *buffer;
    size_t len;
    size_t buflen;
} get_request;

#define CHUNK_SIZE 2048

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    get_request *req = (get_request *)userdata;

    printf("receive chunk of %zu bytes\n", realsize);

    while (req->buflen < req->len + realsize + 1) {
        req->buffer = realloc(req->buffer, req->buflen + CHUNK_SIZE);
        req->buflen += CHUNK_SIZE;
    }
    memcpy(&req->buffer[req->len], ptr, realsize);
    req->len += realsize;
    req->buffer[req->len] = 0;

    return realsize;
}

/* Takes a cjson element and try to fill in the container c;
 * FIXME: currently no error handleling etc; code is unsafe
 */
void cJson_Container(const cJSON *elem, Container *c) {
    const cJSON *image = NULL;
    const cJSON *id = NULL;
    const cJSON *image_id = NULL;
    const cJSON *state = NULL;
    const cJSON *status = NULL;
    const cJSON *created = NULL;
    const cJSON *names = NULL;

    image = cJSON_GetObjectItemCaseSensitive(elem, "Image");
    id = cJSON_GetObjectItemCaseSensitive(elem, "Id");
    image_id = cJSON_GetObjectItemCaseSensitive(elem, "ImageID");
    state = cJSON_GetObjectItemCaseSensitive(elem, "State");
    status = cJSON_GetObjectItemCaseSensitive(elem, "Status");
    created = cJSON_GetObjectItemCaseSensitive(elem, "Created");
    names = cJSON_GetObjectItemCaseSensitive(elem, "Names");

    if (image != NULL) {
        strcpy(c->image, cJSON_GetStringValue(image));
    }

    if (id != NULL) {
        strcpy(c->id, cJSON_GetStringValue(id));
    }

    if (image_id != NULL) {
        strcpy(c->image_id, cJSON_GetStringValue(image_id));
    }

    if (state != NULL) {
        strcpy(c->state, cJSON_GetStringValue(state));
    }

    if (status != NULL) {
        strcpy(c->status, cJSON_GetStringValue(status));
    }

    if (created != NULL) {
        c->created = cJSON_GetNumberValue(created);
    }
}

size_t request_docker_api(get_request *req, char *url) {
    if (req == NULL) {
        printf(stderr, "Request object was not a pointer");
        return -1;
    }

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;


    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, "/var/run/docker.sock"); // FIXME: this must be an configurable variable DOCKER_SOCKET_PATH
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&req);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            printf(stderr, "curl_easy_perform() failed: %s\n",
                   curl_easy_strerror(res));
            return -1;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return req->len;
}

// --------------- API ----------------------
//
/* Returns number of containers. Pointer to a container in memory
 *
 *
 * */
int engine_get_containers(Container **containers, int max_container) {

    if (containers == NULL) {
        return -1;
    }

    get_request req;
    // reset request object to be used again;
    req.buffer = NULL;
    req.len = 0;
    req.buflen = 0;

    size_t amount_request_data = request_docker_api(
        &req, "http://localhost/v1.47/containers/json?all=true");

    if (amount_request_data <= 0) {
        // FIXME: Something went wrong;
        return -1;
    }

    const char *raw_json = (char *)req.buffer;
    const cJSON *response = NULL;
    const cJSON *elem = NULL;

    response = cJSON_Parse(raw_json);
    int containers_amount = cJSON_GetArraySize(response);

    // FIXME: offset in container request
    // this limits the amount of containers to be only the array size;
    // for a proper implementation an offset is needed to render the next
    // page of containers
    if (containers_amount > max_container) {
        containers_amount = max_container;
    }

    for (int i = 0; i < containers_amount; i++) {
        elem = cJSON_GetArrayItem(response, i);
        cJson_Container(elem, containers[i]);
    }

    return containers_amount;
}
