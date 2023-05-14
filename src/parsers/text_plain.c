#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"

static char *_read_file(const char *path) {
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
String *parse_file(const char *pathname, String *str) {
    printf("hello World\n");
    (void)pathname;
    (void)str;
    return NULL;
}
#pragma GCC diagnostic pop
