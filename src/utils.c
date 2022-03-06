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

int randomb(int minimum_number, int max_number){
    return rand() % (max_number +1 - minimum_number) + minimum_number;
}
