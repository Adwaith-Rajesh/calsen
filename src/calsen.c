// calsen.c is the orchestrator
#include "calsen.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/calsenignore.h"
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

static void _tf_table_in_free(void *ht) {
    ht_free_map((HashTable *)ht, tf_table_free_int);
    ht_free((HashTable *)ht);
}

static void *_index_one_file(Node *node, va_list args) {
    FileWithMIME *fwm = (FileWithMIME *)node->data;
    HashTable *parsers = va_arg(args, HashTable *);
    HashTable *tf_table = va_arg(args, HashTable *);
    ParseFileFn *parse_fn = load_parser_entry_point(parsers, fwm->mime_type->str);

    if (parse_fn == NULL) return NULL;
    String *file_contents = string_create(get_file_size(fwm->filepath->str));
    parse_fn(fwm->filepath->str, file_contents);

    if (verbose_flag) {
        printf("indexing: %s\n", fwm->filepath->str);
    }
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
    HashTable *parsers = va_arg(args, HashTable *);
    HashTable *tf_table = va_arg(args, HashTable *);

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
    // text/plain parsers can also parse text/x-c and text/java
    ht_set(parsers, "text_x-c.so", ht_get(parsers, "text_plain.so"));
    ht_set(parsers, "text_x-java.so", ht_get(parsers, "text_plain.so"));

    ll_map(dir_list, _index_one_dir, parsers, file_tf_table);

    dump_index(output_file, file_tf_table);

    ht_drop(parsers, "text_x-c.so");
    ht_drop(parsers, "text_x-java.so");
    ht_free_map(parsers, unload_parser);
    ht_free(parsers);
    ht_free_map(file_tf_table, _tf_table_in_free);
    ht_free(file_tf_table);

    // drop the ignore pattern list cache
    drop_pattern_list_cache();
}

// =========== Search ===========

static void *_idf_token_map(Node *node, va_list args) {
    HashTable *index_table = va_arg(args, HashTable *);
    LinkedList *token_idf_list = va_arg(args, LinkedList *);

    double idf_val = calculate_idf(index_table, (String *)node->data);
    ll_append_left(token_idf_list,
                   create_node(create_token_idf_val((String *)node->data, idf_val)));

    return node->data;
}

static void *_free_token_idf_val_map(Node *node, va_list args) {
    (void)args;
    free_token_idf_val((TokenIDFVal *)node->data);
    return NULL;
}

LinkedList *search(const char *query, const char *index_file) {
    // load the indexed data
    HashTable *index = load_index(index_file);

    // list to store all the tokens along with its idf vals
    LinkedList *token_idf_list = ll_init();

    // parse the query
    // we are just gonna use the same function that tokenize the file content to
    // tokenize the query.
    int tw_var = 0;
    LinkedList *query_tokens = file_content_to_tokens((char *)query, strlen(query), &tw_var);

    // gets the number of files the token appeared in the corpus
    ll_map(query_tokens, _idf_token_map, index, token_idf_list);

    // do IDF
    // linked list of FileTFIDFVal *
    LinkedList *file_tf_idf_list = calculate_tf_idf(index, token_idf_list);

    ll_map(query_tokens, _free_string_from_list);
    ll_free(query_tokens);

    // free the loaded index
    ht_free_map(index, _tf_table_in_free);
    ht_free(index);

    // free the token_idf_list
    ll_map(token_idf_list, _free_token_idf_val_map);
    ll_free(token_idf_list);

    return file_tf_idf_list;
}
