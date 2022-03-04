#ifndef __utilsh__
#define __utilsh__
#include <stddef.h>

typedef enum{ false = 0, true = 1 } bool;

void* safe_malloc(size_t size);

void* safe_calloc(size_t nmemb, size_t size);

#endif
