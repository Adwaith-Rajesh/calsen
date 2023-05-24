// calsen.c is the orchestrator
#include "calsen.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "hash_table.h"
#include "indexer.h"
#include "lexer.h"
#include "linked_list.h"
#include "load_parser.h"
#include "path.h"
#include "tf_idf.h"

extern int verbose_flag;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

// useful debugging function

static void _node_filepath_printer(Node *node) {
    if (node == NULL) return;
    string_print(((FileWithMIME *)node->data)->filepath);
    printf("\n");
}

#pragma GCC diagnostic pop

static void *_free_string_from_list(Node *node, va_list args) {
    (void)args;
    if (node == NULL) return NULL;
    string_destroy((String *)node->data);
    return NULL;
}

static void *_free_fwm_from_list(Node *node, va_list args) {
    (void)args;
    if (node == NULL) return NULL;
    free_file_with_mime((FileWithMIME *)node->data);
    return NULL;
}

static void tf_table_in_free(void *ht) {
    ht_free_map((HashTable *)ht, tf_table_free_int);
    ht_free((HashTable *)ht);
}

static void *_index_one_file(Node *node, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    FileWithMIME *fwm = (FileWithMIME *)node->data;
    HashTable *parsers = va_arg(args_copy, HashTable *);
    HashTable *tf_table = va_arg(args_copy, HashTable *);

    if (verbose_flag) {
        printf("indexing: %s\n", fwm->filepath->str);
    }
    ParseFileFn *parse_fn = load_parser_entry_point(parsers, fwm->mime_type->str);
    if (parse_fn == NULL) return NULL;
    String *file_contents = string_create(get_file_size(fwm->filepath->str));
    parse_fn(fwm->filepath->str, file_contents);

    // get the tokens
    int original_token_count = 0;
    LinkedList *file_tokens = file_content_to_tokens(file_contents->str,
                                                     file_contents->size, &original_token_count);

    HashTable *tf_vals = token_count(file_tokens);
    calculate_tf(tf_vals, original_token_count);

    ht_set(tf_table, fwm->filepath->str, tf_vals);

    ll_map(file_tokens, _free_string_from_list);
    ll_free(file_tokens);
    string_destroy(file_contents);
    return node->data;
}

static void *_index_one_dir(Node *node, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    HashTable *parsers = va_arg(args_copy, HashTable *);
    HashTable *tf_table = va_arg(args_copy, HashTable *);

    // get all the file to index along with their MIME type
    LinkedList *files_to_index = ll_init();
    String *dirname = (String *)node->data;
    get_files_to_index(dirname->str, files_to_index);

    // index each file one by one
    ll_map(files_to_index, _index_one_file, parsers, tf_table);

    ll_map(files_to_index, _free_fwm_from_list);
    ll_free(files_to_index);
    return node->data;
}

void calsen_index_files(LinkedList *dir_list, const char *output_file) {
    HashTable *file_tf_table = ht_create();

    HashTable *parsers = load_all_parsers();
    // text/plain parsers can also parse text/x-c
    ht_set(parsers, "text_x-c.so", ht_get(parsers, "text_plain.so"));

    ll_map(dir_list, _index_one_dir, parsers, file_tf_table);

    dump_index(output_file, file_tf_table);
    ht_free_map(parsers, unload_parser);
    ht_free(parsers);
    ht_free_map(file_tf_table, tf_table_in_free);
    ht_free(file_tf_table);
}
