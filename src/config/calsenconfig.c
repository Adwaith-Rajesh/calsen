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
// the third line will be the index file
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

    n_bytes = _read_line(fp, temp_path);
    if (n_bytes == 0) _print_config_error("path to index file not specified in the config file %s\n", filepath);
    strcpy(c->index_file, temp_path);
    fclose(fp);
}

/*
if a filepath was passed during compilation then the file will checked and parsed, if the file
path is invalid then the program stops instantly
all the parsed configs can be overriden by env vars, and those can be overriden by the CLI
 */

config_t config;  // a simple cache
int config_set = 0;

config_t *get_calsen_config() {
    if (config_set) return &config;

    char *parsers_dir = getenv("CALSEN_PARSER_DIR");
    char *ignore_file = getenv("CALSENIGNORE");
    char *index_file = getenv("CALSEN_INDEX");
    char *passed_config = CALSENCONFIG;

    char resolved_path[PATH_MAX] = {0};

    if (LOAD_CONFIG)
        strcpy(config.config_file, get_absolute_path(passed_config, resolved_path));
    // the absolute path to index cannot be found as the file does not exist
    // when the calsen runs for the first time
    if (LOAD_CONFIG && (parsers_dir == NULL || ignore_file == NULL || index_file == NULL)) {
        _parse_config_file(passed_config, &config);
    }

    if (!LOAD_CONFIG && (parsers_dir == NULL || ignore_file == NULL || index_file == NULL)) {
        fprintf(stderr,
                "\x1b[36mone of CALSEN_PARSER_DIR, CALSENIGNORE, CALSEN_INDEX "
                "not set, switching to default values.\n\x1b[0m");
        if (parsers_dir == NULL) strcpy(config.parsers_dir, "./build/parsers");
        if (ignore_file == NULL) strcpy(config.ignore_file, "./calsenconfig");
        if (index_file == NULL) strcpy(config.index_file, "./calsen.index");

        fprintf(stderr, "\x1b[36mCALSEN_PARSER_DIR=%s\nCALSENIGNORE=%s\nCALSEN_INDEX=%s\n\x1b[0m",
                config.parsers_dir, config.ignore_file, config.index_file);
    }

    if (parsers_dir) strcpy(config.parsers_dir, get_absolute_path(parsers_dir, resolved_path));
    if (ignore_file) strcpy(config.ignore_file, get_absolute_path(ignore_file, resolved_path));
    if (index_file) strcpy(config.index_file, index_file);
    config_set = 1;
    return &config;
}
