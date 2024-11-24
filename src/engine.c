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

/* Returns number of containers. Pointer to a container in memory
 *
 *
 * */
int request_docker_api(Container *c) {

    // return error if container pointer is not set correctly
    if (c == NULL) {
        return -1;
    }

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    get_request req = {
        .buffer = NULL, .len = 0, .buflen = 0}; // empty get_requets

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

    const char *raw_json = (const char *)req.buffer;
    const cJSON *response = NULL;
    const cJSON *elem = NULL;
    const cJSON *image = NULL;
    const cJSON *id = NULL;
    const cJSON *image_id = NULL;
    const cJSON *state = NULL;
    const cJSON *status = NULL;
    const cJSON *created = NULL;
    const cJSON *names = NULL;

    response = cJSON_Parse(raw_json);
    elem = cJSON_GetArrayItem(response, 0);
    image = cJSON_GetObjectItemCaseSensitive(elem, "Image");
    id = cJSON_GetObjectItemCaseSensitive(elem, "Id");
    image_id = cJSON_GetObjectItemCaseSensitive(elem, "ImageId");
    state = cJSON_GetObjectItemCaseSensitive(elem, "State");
    status = cJSON_GetObjectItemCaseSensitive(elem, "Status");
    created = cJSON_GetObjectItemCaseSensitive(elem, "Created");
    names = cJSON_GetObjectItemCaseSensitive(elem, "Names");

    c->image = cJSON_GetStringValue(image);
    c->id = cJSON_GetStringValue(id);
    c->image_id = cJSON_GetStringValue(image_id);
    c->state = cJSON_GetStringValue(state);
    c->status = cJSON_GetStringValue(status);
    c->created = cJSON_GetNumberValue(created);
    return 1;
}
