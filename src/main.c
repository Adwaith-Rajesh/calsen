/*
Calsen. A search engine for files
    Copyright (C) 2023  Adwaith Rajesh

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calsen.h"
#include "config/calsenconfig.h"
#include "cstring.h"
#include "linked_list.h"
#include "path.h"
#include "tf_idf.h"

int verbose_flag;

static LinkedList *_ll_check_null_add(LinkedList *list, char *dirname) {
    // we kind of have to own dirname, do it's better to convert it to a String *
    if (list == NULL) {
        list = ll_init();
    }
    ll_append_left(list, create_node(
                             string_create_from_charp(dirname, strlen(dirname))));
    return list;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

// useful debugging function

static void _node_string_printer(Node *node) {
    if (node == NULL) return;
    string_print((String *)node->data);
    printf("\n");
}

#pragma GCC diagnostic pop

static void *_ll_string_destroy(Node *node, va_list args) {
    (void)args;
    if (node == NULL) return NULL;
    string_destroy((String *)node->data);

    return NULL;
}

static void _ll_print_file_tf_idf_map(Node *node) {
    if (node == NULL) return;

    if (verbose_flag)
        printf("%.12lf : ", ((FileTFIDFVal *)node->data)->tf_idf_val);
    string_print((String *)((FileTFIDFVal *)node->data)->filename);
    printf("\n");
}

static void *_ll_free_file_tf_idf_map(Node *node, va_list args) {
    (void)args;
    free_file_tf_idf_val((FileTFIDFVal *)node->data);
    return NULL;
}

void print_help(FILE *stream, int type, char *filename) {
    char *simple_usage =
        "Usage: %s <search/reindex> [OPTIONS]\n"
        "--help, -h \tShow this message and quit.\n";

    char *reindex_usage =
        "Usage: %s reindex [OPTIONS]\n"
        "--output, -o \tThe file to output the indexed data to.\n"
        "--verbose, -v \tGet verbose output.\n"
        "--dir, -d \tThe directories to find the files to index.\n"
        "--ignore, -g \tPath to the ignore file, overrides the config file"
        "--help, -h \tShow this message and quit.\n";
    char *search_usage =
        "Usage: %s search [OPTIONS]\n"
        "--index, -i \tThe file that contains the indexed data.\n"
        "--query, -q \tThe query to search.\n"
        "--verbose, -v \tGet verbose output.\n"
        "--number, -n \tNumber of results required."
        "--help, -h \tShow this message and quit.\n";

    switch (type) {
        case 2:
            fprintf(stream, reindex_usage, filename);
            break;
        case 3:
            fprintf(stream, search_usage, filename);
            break;
        default:  // this include case 1
            fprintf(stream, simple_usage, filename);
            break;
    }
}

int main(int argc, char **argv) {
    // all the options are parses first then the sub commands will be checked

    // --dir -d  --> the dir to include during indexing
    // --verbose -v --> give info on the current state / more output
    // --help -h --> show help and quit
    // --output -o --> the output index file for the index subcommand
    // --index -i -->  the index file for the search subcommand
    // --query, -q -> The query to search for
    // subcommand:
    //      reindex
    //      search
    int help_val = 0;
    int c;

    config_t *config = get_calsen_config();

    String *output_file = NULL;
    String *index_file = NULL;
    String *argv_1_val = NULL;
    String *query = NULL;
    String *ignore_file = NULL;
    LinkedList *dir_list = NULL;
    int n_results = 0;

    struct option long_options[] = {
        {"verbose", no_argument, &verbose_flag, 1},
        {"help", no_argument, &help_val, 1},
        {"output", required_argument, 0, 'o'},
        {"index", required_argument, 0, 'i'},
        {"dir", required_argument, 0, 'd'},
        {"query", required_argument, 0, 'q'},
        {"number", optional_argument, 0, 'n'},
        {"ignore", required_argument, 0, 'g'},
        {0, 0, 0, 0},
    };

    if (argc >= 2) {
        argv_1_val = string_create_from_charp(argv[1], strlen(argv[1]));
        // for some reason the code below seems to change the order
    }

    while (1) {
        int options_index = 0;
        c = getopt_long(argc, argv, "o:i:d:q:n:g:", long_options, &options_index);

        if (c == -1) break;

        switch (c) {
            case 'i':
                index_file = string_create_from_charp(optarg, strlen(optarg));
                break;
            case 'o':
                output_file = string_create_from_charp(optarg, strlen(optarg));
                break;
            case 'd':
                dir_list = _ll_check_null_add(dir_list, optarg);
                break;
            case 'q':
                query = string_create_from_charp(optarg, strlen(optarg));
                break;
            case 'n':
                n_results = atoi(optarg);
                break;
            case 'g':
                ignore_file = string_create_from_charp(optarg, strlen(optarg));
                break;
        }
    }

    if (argc <= 2 && help_val) {
        print_help(stdout, 1, argv[0]);
        exit(0);
    }

    if (argc < 2) {
        print_help(stderr, 1, argv[0]);
        exit(1);
    }

    /*
        if the index file or the output file is not specifed then the
        filepath provided in the config file will be used.
        if the index file or the output file is specifed then the index file
        path specified in the config file will be ignored and replaced
    */

    if (index_file == NULL) {
        index_file = string_create_from_charp(config->index_file, strlen(config->index_file));
    } else {
        strcpy(config->index_file, index_file->str);
    }

    if (output_file == NULL) {
        output_file = string_create_from_charp(config->index_file, strlen(config->index_file));
    } else {
        strcpy(config->index_file, output_file->str);
    }

    if (ignore_file != NULL) {
        char resolved_path[PATH_MAX];
        strcpy(config->ignore_file, get_absolute_path(ignore_file->str, resolved_path));
    }

    if (strcmp("reindex", argv_1_val->str) == 0) {
        if ((dir_list == NULL) || (output_file == NULL)) {
            fprintf(stderr, "reindex: missing one of --dir, --index\n");
            print_help(stderr, 2, argv[0]);
            exit(1);
        }
        calsen_index_files(dir_list, output_file->str);
        ll_map(dir_list, _ll_string_destroy);
        ll_free(dir_list);
    }

    if (strcmp("search", argv_1_val->str) == 0) {
        if ((query == NULL) || (index_file == NULL)) {
            fprintf(stderr, "search: missing one of --query, --output");
            print_help(stderr, 3, argv[0]);
            exit(1);
        }

        LinkedList *file_list = search(query->str, index_file->str);
        LinkedList *sorted_filter_list = filter_sort_file_tf_idf_list(file_list, n_results);
        ll_print(sorted_filter_list, _ll_print_file_tf_idf_map);
        ll_map(file_list, _ll_free_file_tf_idf_map);
        ll_free(file_list);
    }

    string_destroy(output_file);
    string_destroy(index_file);
    string_destroy(argv_1_val);
    string_destroy(query);
    string_destroy(ignore_file);

    return 0;
}
