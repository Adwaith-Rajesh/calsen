#define _XOPEN_SOURCE 500

#include <dirent.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define _GNU_SOURCE
#include <dlfcn.h>

#include "config/calsenconfig.h"
#include "cstring.h"
#include "hash_table.h"
#include "load_parser.h"
#include "path.h"

static void _load_lib_to_table(const char *lib_path, const char *lib_name, HashTable *table) {
    void *lib_handle = dlopen(lib_path, RTLD_LAZY);
    if (lib_handle == NULL) return;
    ht_set(table, lib_name, lib_handle);
}

HashTable *load_all_parsers() {
    config_t *config = get_calsen_config();

    char *parsers_dir = config->parsers_dir;
    if (!is_dir(parsers_dir)) {
        fprintf(stderr, "'%s' is not a directory\n", parsers_dir);
        exit(EXIT_FAILURE);
    }

    // find all the so files in the dir
    // open them, and then store it in a HashTable
    struct dirent *d;
    DIR *dir = opendir(parsers_dir);

    if (dir == NULL) {
        perror("Could not open directory");
        exit(EXIT_FAILURE);
    }

    HashTable *ht = ht_create();
    String *shared_lib_path;
    while ((d = readdir(dir)) != NULL) {
        // check if file name ends with so
        char *dot = strrchr(d->d_name, '.');  // find the last dot(.)

        if (dot == NULL) continue;
        shared_lib_path = string_create_from_charp(parsers_dir, PATH_MAX);
        if (strcmp(dot, ".so") == 0) {
            string_append_char(shared_lib_path, '/');
            string_append_charp(shared_lib_path, d->d_name);
            _load_lib_to_table(shared_lib_path->str, d->d_name, ht);
        }
        string_destroy(shared_lib_path);
    }
    closedir(dir);
    return ht;
}

ParseFileFn *load_parser_entry_point(HashTable *table, char *mime) {
    // // mime convert: type/plain -> type_plain
    // char *sl = strrchr(mime, '/');
    // if (sl != NULL) mime[(int)(sl - mime)] = '_';

    for (size_t i = 0; i < strlen(mime); i++) {
        if (mime[i] == '/')
            mime[i] = '_';
    }

    String *str = string_create_from_charp(mime, 50);
    string_append_charp(str, ".so");
    void *lib_handle = ht_get(table, str->str);
    string_destroy(str);

    if (lib_handle == NULL) return NULL;
    return (ParseFileFn *)dlsym(lib_handle, "parse_file");
}

void unload_parser(void *lib_handle) {
    if (lib_handle == NULL) return;
    dlclose(lib_handle);
}
