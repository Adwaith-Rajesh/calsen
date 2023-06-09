#include "calsenignore.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"
#include "linked_list.h"

LinkedList *g_pattern_list = NULL;  // simple cache

static void *_free_string_patter_list(Node *node, va_list args) {
    (void)args;
    string_destroy((String *)node->data);
    return NULL;
}

void drop_pattern_list_cache() {
    ll_map(g_pattern_list, _free_string_patter_list);
    ll_free(g_pattern_list);
    g_pattern_list = NULL;
}

// returns: LikedList[String]  i.e. a linked list of patterns to match
LinkedList *parse_ignore_file(const char *filepath) {
    if (g_pattern_list != NULL) return g_pattern_list;

    FILE *fp = fopen(filepath, "r");

    if (fp == NULL) {
        fprintf(stderr, "WARN: could not open: %s : %s\n", filepath, strerror(errno));
        return NULL;
    }

    LinkedList *pattern_list = ll_init();

    // read character by character until we reach newline or '#'
    // we will then consider that string as a pattern
    int ch;
    String *pattern = string_create(20);
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '#')
            while ((ch != '\n') && (ch != EOF)) {
                ch = fgetc(fp);
            }

        if (ch == '\n') {
            if (pattern->curr_p != 0) {
                String *strip_pattern = string_strip(pattern, 0);
                ll_append_left(pattern_list, create_node(strip_pattern));
                string_reset(pattern);
            }
            continue;
        }
        pattern = string_expandable_append(pattern, ch);
    }
    if (pattern->curr_p != 0) ll_append_left(pattern_list,
                                             create_node(string_strip(pattern, 0)));
    string_destroy(pattern);
    fclose(fp);
    g_pattern_list = pattern_list;  // set the cache
    return pattern_list;
}

static int _check_pattern_match(const char *pattern, const char *str) {
    if (*pattern == '\0' && *str == '\0') return 1;
    if (*pattern == '\0') return 0;
    if (*str == '\0') {
        if (*pattern != '*') return 0;
        return _check_pattern_match(pattern + 1, str);
    }
    if (*pattern == '?' || *pattern == *str) return _check_pattern_match(pattern + 1, str + 1);
    if (*pattern == '*') return _check_pattern_match(pattern + 1, str) || _check_pattern_match(pattern, str + 1);
    return 0;
}

int check_file_name_is_ignored(LinkedList *patterns, const char *str) {
    for (Node *node = patterns->head; node != NULL; node = node->next) {
        if (_check_pattern_match(((String *)node->data)->str, str)) return 1;
    }
    return 0;
}
