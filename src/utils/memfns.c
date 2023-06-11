#include "memfns.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *malloc_with_check(size_t n_bytes) {
    void *mem = malloc(n_bytes);
    if (mem == NULL) {
        fprintf(stderr, "ERROR: malloc: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return mem;
}
