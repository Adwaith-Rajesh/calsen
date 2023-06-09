#include "calsenignore.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"
#include "linked_list.h"

static void _node_printer(Node *node) {
    string_print((String *)node->data);
}

// returns: LikedList[String]  i.e. a linked list of patterns to match
LinkedList *parse_ignore_file(const char *filepath) {
    FILE *fp = fopen(filepath, "r");

    if (fp == NULL) {
        fprintf(stderr, "WARN: could not open: %s : %s\n", filepath, strerror(errno));
        return NULL;
    }

    LinkedList *pattern_list = ll_init();

    if (fp == NULL) {
        fprintf(stderr, "ERROR: could not open %s : %s\n", filepath, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // read character by character until we reach newline or '#'
    // we will then consider that string as a patter
    int ch;
    String *pattern = string_create(20);
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '#')
            while ((ch != '\n') && (ch != EOF)) {
                ch = fgetc(fp);
                printf("char: %d\n", ch);
            }

        if (ch == '\n') {
            if (pattern->size != 0) {
                ll_append_left(pattern_list, create_node(string_create_from_charp(pattern->str, pattern->size)));
                string_reset(pattern);
                continue;
            }
        }
        pattern = string_expandable_append(pattern, ch);
    }
    string_destroy(pattern);
    fclose(fp);
    return pattern_list;
}
