#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void parse_file(const char *pathname, String *str) {
    FILE *fp = fopen(pathname, "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file: %s : %s\n", pathname, strerror(errno));
        exit(1);
    }
    size_t read_bytes = fread(str->str, str->size, 1, fp);
    if (read_bytes == 0) {
        fprintf(stderr, "Error, reading contents of: %s", pathname);
    }
    fclose(fp);
}
#pragma GCC diagnostic pop
