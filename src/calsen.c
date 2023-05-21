// calsen.c is the orchestrator
#include "calsen.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "hash_table.h"
#include "indexer.h"
#include "linked_list.h"
#include "load_parser.h"
#include "path.h"

extern int verbose_flag;

static void _node_filepath_printer(Node *node) {
    if (node == NULL) return;
    string_print(((FileWithMIME *)node->data)->filepath);
    printf("\n");
}

static void *_free_string_from_list(Node *node) {
    if (node == NULL) return NULL;
    string_destroy((String *)node->data);
    return NULL;
}

static void *_index_one_file(Node *node, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    FileWithMIME *fwm = (FileWithMIME *)node->data;
    HashTable *parsers = va_arg(args_copy, HashTable *);

    if (verbose_flag) {
        printf("indexing: %s\n", fwm->filepath->str);
    }
    ParseFileFn *parse_fn = load_parser_entry_point(parsers, fwm->mime_type->str);
    if (parse_fn == NULL) return NULL;
    String *file_contents = string_create(get_file_size(fwm->filepath->str));
    parse_fn(fwm->filepath->str, file_contents);

    string_print(file_contents);
    return node->data;
}

static void *_index_one_dir(Node *node, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    HashTable *parsers = va_arg(args_copy, HashTable *);

    // get all the file to index along with their MIME type
    LinkedList *files_to_index = ll_init();
    String *dirname = (String *)node->data;
    get_files_to_index(dirname->str, files_to_index);
    ll_print(files_to_index, _node_filepath_printer);

    ll_map(files_to_index, _index_one_file, parsers);

    return node->data;
}

void calsen_index_files(LinkedList *dir_list) {
    HashTable *parsers = load_all_parsers();
    ht_set(parsers, "text_x-c.so", ht_get(parsers, "text_plain.so"));

    ll_map(dir_list, _index_one_dir, parsers);
    ht_free_map(parsers, unload_parser);
}
