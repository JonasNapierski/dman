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

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

int request_docker_api() {
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
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            return -1;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}
