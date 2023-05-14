#include "path.h"

#define _XOPEN_SOURCE 500

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int is_dir(const char *pathname) {
    struct stat ps;

    if (stat(pathname, &ps) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    return S_ISDIR(ps.st_mode);
}

char *get_absolute_path(const char *path, char *resolved_path) {
    char *r = realpath(path, resolved_path);
    if (r == NULL) {
        perror("could not find absolute path");
        exit(EXIT_FAILURE);
    }
    return r;
}

int get_file_size(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        fprintf(stderr, "could not find file: %s : %s\n", path, strerror(errno));
        exit(1);
    }
    return st.st_size;
}
