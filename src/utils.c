#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

void* safe_malloc(size_t size){
    void *p = malloc(size);
    if (p == NULL){
        fprintf(stderr, "ERROR: failed to allocate %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void* safe_calloc(size_t nmemb, size_t size){
    void *p = calloc(nmemb, size);
    if(p == NULL) {
        fprintf(stderr, "ERROR: failed to allocate %zu bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}
