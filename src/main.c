/*
MIT License

Copyright (c) 2023 Adwaith Rajesh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calsen.h"
#include "cstring.h"
#include "linked_list.h"

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
void print_help(FILE *stream, int type, char *filename) {
    char *simple_usage =
        "Usage: %s <search/reindex> [OPTIONS]\n"
        "--help, -h \tShow this message and quit.\n";

    char *reindex_usage =
        "Usage: %s reindex [OPTIONS]\n"
        "--output, -o \tThe file to output the indexed data to.\n"
        "--verbose, -v \tGet verbose output.\n"
        "--dir, -d \tThe directories to find the files to index.\n"
        "--help, -h \tShow this message and quit.\n";
    char *search_usage =
        "Usage: %s search [OPTIONS]\n"
        "--index, -i \tThe file that contains the indexed data.\n"
        "--query, -q \tThe query to search.\n"
        "--verbose, -v \tGet verbose output.\n"
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

    String *output_file = NULL;
    String *index_file = NULL;
    String *argv_1_val = NULL;
    String *query = NULL;
    LinkedList *dir_list = NULL;

    struct option long_options[] = {
        {"verbose", no_argument, &verbose_flag, 1},
        {"help", no_argument, &help_val, 1},
        {"output", required_argument, 0, 'o'},
        {"index", required_argument, 0, 'i'},
        {"dir", required_argument, 0, 'd'},
        {"query", required_argument, 0, 'q'},
        {0, 0, 0, 0},
    };

    if (argc >= 2) {
        argv_1_val = string_create_from_charp(argv[1], strlen(argv[1]));
        // for some reason the code below seems to change the order
    }

    while (1) {
        int options_index = 0;
        c = getopt_long(argc, argv, "o:i:d:q:", long_options, &options_index);

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

    if (strcmp("reindex", argv_1_val->str) == 0) {
        if ((dir_list == NULL) || (output_file == NULL)) {
            fprintf(stderr, "reindex: missing one of --dir, --output\n");
            print_help(stderr, 2, argv[0]);
            exit(1);
        }
        calsen_index_files(dir_list, output_file->str);
        ll_map(dir_list, _ll_string_destroy);
        ll_free(dir_list);
    }

    if (strcmp("search", argv_1_val->str) == 0) {
        if ((query == NULL) || (index_file == NULL)) {
            fprintf(stderr, "search: missing one of --query, --index\n");
            print_help(stderr, 3, argv[0]);
            exit(1);
        }
    }

    string_destroy(output_file);
    string_destroy(index_file);
    string_destroy(argv_1_val);
    string_destroy(query);

    return 0;
}
