// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>

#include "calsenconfig.h"

#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"

// the config file for now is pretty straight forward.
// this first line will the parsers dir.
// the second line will be the ignore file path
static int _read_line(FILE *fp, char buf[PATH_MAX]) {
    // reads a line until \n, return the number of bytes read
    int n_bytes = 0;
    int ch;
    while (((ch = fgetc(fp)) != EOF && (ch != '\n')) && n_bytes < PATH_MAX) {
        buf[n_bytes++] = ch;
    }
    buf[n_bytes] = '\0';
    return n_bytes;
}

static void _print_config_error(const char *error_msg, const char *config_file_path) {
    fprintf(stderr, error_msg, config_file_path);
    exit(EXIT_FAILURE);
}

static void _parse_config_file(const char *filepath, config_t *c) {
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: could not read config file %s : %s\n", filepath, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // try to read two lines
    int n_bytes;
    char temp_path[PATH_MAX];
    char resolved_path[PATH_MAX];

    n_bytes = _read_line(fp, temp_path);
    if (n_bytes == 0) _print_config_error("path to parsers dir not specified in the config file %s\n", filepath);
    strcpy(c->parsers_dir, get_absolute_path(temp_path, resolved_path));

    n_bytes = _read_line(fp, temp_path);
    if (n_bytes == 0) _print_config_error("path to .calsenignore file not specified in the config file %s\n", filepath);
    strcpy(c->ignore_file, get_absolute_path(temp_path, resolved_path));
}

static void _create_config_file(const char *filepath, config_t *c) {
}

/*
if the config file passed during compilation is "", we will check for the existence
of ~/.config/calsen/.calsenconfig. If does not exist the file will be crated with default values,

if CALSENIGNORE and CALSEN_PARSER_DIR is present as environment variables the file will not be
parsed

if a filepath was passed during compilation then the file will checked and parsed, if the file
path is invalid then the program stops instantly.
 */
config_t get_calsen_config() {
    char *parsers_dir = getenv("CALSEN_PARSER_DIR");
    char *ignore_file = getenv("CALSENIGNORE");
    char *passed_config = CALSENCONFIG;

    config_t c;
    char resolved_path[PATH_MAX] = {0};
    strcpy(c.config_file, get_absolute_path(passed_config, resolved_path));

    if (parsers_dir != NULL && ignore_file != NULL) {
        strcpy(c.parsers_dir, get_absolute_path(parsers_dir, resolved_path));
        strcpy(c.ignore_file, get_absolute_path(ignore_file, resolved_path));
        return c;
    }

    _parse_config_file(passed_config, &c);

    return c;
}
