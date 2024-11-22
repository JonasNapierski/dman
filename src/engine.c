// NOTES for v0.1.0
//
// create a structure for the container
// - uuid
// - name
// - status
//
// method stop,start list all container

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

int request_docker_api() {
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

    response = cJSON_Parse(raw_json);
    elem = cJSON_GetArrayItem(response, 0);
    image = cJSON_GetObjectItemCaseSensitive(elem, "Image");
    printf("Array len: %i", cJSON_GetArraySize(response));
    printf("Object len: %i", cJSON_GetArraySize(elem));
    printf("Name: %s", cJSON_GetStringValue(image));
    return 0;
}
