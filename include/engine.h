// NOTES for v0.1.0
//
// create a structure for the container
// - uuid
// - name
// - status
//
// method stop,start list all container
//

#ifndef ENGINE_H
#define ENGINE_H

#define CONTAINER_MAX_STRING 255
#define CONTAINER_MAX_NAMES 16

typedef struct {
    char id[CONTAINER_MAX_STRING];
    char image[CONTAINER_MAX_STRING];
    char image_id[CONTAINER_MAX_STRING];
    char state[CONTAINER_MAX_STRING];
    char status[CONTAINER_MAX_STRING];
    double created;
    char names[CONTAINER_MAX_STRING][CONTAINER_MAX_NAMES];

} Container;

int request_docker_api(Container *c);
#endif
