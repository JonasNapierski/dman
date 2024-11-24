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

typedef struct {
    char *id;
    char *image;
    char *image_id;
    char *state;
    char *status;
    unsigned int created;
    char **names;

} Container;

int request_docker_api(Container *c);
#endif
