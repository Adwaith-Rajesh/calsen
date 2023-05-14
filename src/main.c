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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "lexer.h"
#include "linked_list.h"
#include "load_parser.h"
#include "path.h"

void node_printer(Node *node) {
    if (node == NULL) return;
    printf("tok: ");
    string_print((String *)node->data);
    // printf("%p\n", node->data);
    printf("\n");
}

void *free_string_token(Node *node, va_list list) {
    (void)list;
    if (node == NULL) return NULL;
    string_destroy(node->data);
    return node;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./calsen <filepath>\n");
        exit(1);
    }
    const char *filepath = argv[1];

    HashTable *parsers = load_all_parsers();

    // ParseFileFn *fn = loa
    char mime_type[] = "text/plain";
    // printf("fn -> %p\n", load_parser_entry_point(parsers, mime_type));

    // printf("size-> %d\n", get_file_size(filepath));

    String *str = string_create(get_file_size(filepath) + 1);

    load_parser_entry_point(parsers, mime_type)(filepath, str);
    string_print(str);

    LinkedList *tok_list = file_content_to_tokens(str->str, str->size);
    ll_print(tok_list, node_printer);

    string_destroy(str);
    ll_map(tok_list, free_string_token);
    ll_free(tok_list);
    ht_free_map(parsers, unload_parser);
    ht_free(parsers);

    return EXIT_SUCCESS;
}
