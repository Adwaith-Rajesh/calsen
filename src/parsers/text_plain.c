#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"

static char *_read_file(const char *path) {
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
parse_file(const char *pathname, String *str) {
    FILE *fp = fopen(pathname, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file: %s : %s\n", pathname, strerror(errno));
        exit(1);
    }
    fread(str->str, str->size, 1, fp);
    fclose(fp);
}
#pragma GCC diagnostic pop
