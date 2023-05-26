#include "tf_idf.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"
#include "hash_table.h"
#include "linked_list.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static void _node_printer(Node *node) {
    if (node == NULL) return;
    string_print((String *)node->data);
    printf("\n");
}

#pragma GCC diagnostic pop

static inline String *_node_val_to_string(void *node) {
    return ((String *)(((Node *)node)->data));
}

/*
If a value is not in the hash table, then key is added to the
hash table with the value being 1, which allocated in the heap
*/
static void _allocate_and_init(HashTable *ht, char *key) {
    int *val = malloc(sizeof(int));
    *val = 1;
    ht_set(ht, key, val);
}

/*
calculates the TF score of all the value in the token LinkedList.
The value the HashTable including the value it holds has to be freed.

void tf_table_free_int(void *);

by mapping the function onto the table
*/
HashTable *token_count(LinkedList *token_list) {
    HashTable *ht = ht_create();
    LLIter *iter = ll_iter_init(token_list);

    void *next_val = ll_iter_next(iter);
    while (next_val != NULL) {
        // check if it exists in the hash table
        void *entry = ht_get(ht, _node_val_to_string(next_val)->str);
        if (entry == NULL) {
            _allocate_and_init(ht, _node_val_to_string(next_val)->str);
            next_val = ll_iter_next(iter);
            continue;
        }
        (*((int *)entry))++;  // increment the existing value
        next_val = ll_iter_next(iter);
    }

    ll_iter_free(iter);
    return ht;
}

static void _calculate_tf_value_for_entry(HTEntry *entry, va_list args) {
    size_t table_size = va_arg(args, size_t);

    int token_count = *((int *)(entry->value));
    free(entry->value);  // free the existing int value
    // create space for the new float value to be stores in the  entry
    double *tf_val = malloc(sizeof(double));
    *tf_val = (double)token_count / (double)table_size;
    entry->value = tf_val;
}

void calculate_tf(HashTable *tf_values, int token_count) {
    ht_entry_map(tf_values, _calculate_tf_value_for_entry, token_count);
}

void tf_table_free_int(void *int_val) {
    if (int_val == NULL) return;
    free(int_val);
}

// =========== IDF stuff ===========

static void _tf_table_iter_inner(HTEntry *entry, va_list args) {
    int *t_count = va_arg(args, int *);
    String *token = va_arg(args, String *);

    if (strcmp(entry->key, token->str) == 0) *t_count = *t_count + 1;
}

static void _tf_table_iter(HTEntry *entry, va_list args) {
    int *t_count = va_arg(args, int *);
    String *token = va_arg(args, String *);

    ht_entry_map(entry->value, _tf_table_iter_inner, t_count, token);
}

// index_table -> the entire index
double calculate_idf(HashTable *index_table, String *token) {
    size_t n_docs = ht_get_size(index_table);

    // count the number of files the token exist in
    int t_count = 0;
    ht_entry_map(index_table, _tf_table_iter, &t_count, token);

    // printf("docs count: %ld\n", n_docs);
    // printf("token count: %d\n", t_count);

    return log10(((double)n_docs / ((double)(1 + t_count))));
}

// =========== Storing the values  ===========

TokenIDFVal *create_token_idf_val(String *token, double idf_val) {
    TokenIDFVal *new_t_idf_val = malloc(sizeof(TokenIDFVal));
    new_t_idf_val->token = token;
    new_t_idf_val->idf_val = idf_val;
    return new_t_idf_val;
}

FileTFIDFVal *create_file_tf_idf_val(String *filename, double tf_idf_val) {
    FileTFIDFVal *new_ft_idf_val = malloc(sizeof(FileTFIDFVal));
    new_ft_idf_val->filename = string_create_from_charp(filename->str, filename->size);
    new_ft_idf_val->tf_idf_val = tf_idf_val;
    return new_ft_idf_val;
}

void free_token_idf_val(TokenIDFVal *t_idf_val) {
    if (t_idf_val == NULL) return;
    free(t_idf_val);
}

void free_file_tf_idf_val(FileTFIDFVal *ft_idf_val) {
    if (ft_idf_val == NULL) return;

    // FileTFIDFval owns the filename String *
    string_destroy(ft_idf_val->filename);
    free(ft_idf_val);
}

// =========== TF-IDF  ===========

// pick a file from the index, then for each token in the token_idf_list
// do the following
// file_tf_idf += tf_val_token * idf_val_token

static inline TokenIDFVal *_node_to_tidf_val(Node *node) {
    return (TokenIDFVal *)node->data;
}

static void _tf_idf_file_map(HTEntry *entry, va_list args) {
    if (entry == NULL) return;  // just in case

    LinkedList *token_idf_list = va_arg(args, LinkedList *);
    LinkedList *file_tf_idf_list = va_arg(args, LinkedList *);

    double file_tf_idf_val = 0.0;

    // tf_index of one file
    HashTable *tf_file_index = (HashTable *)entry->value;

    printf("filename: %s\n", entry->key);

    // each node holds TokenIDFVal *
    Node *token = token_idf_list->head;
    for (; token != NULL; token = token->next) {
        double *token_tf_val = ht_get(tf_file_index, _node_to_tidf_val(token)->token->str);
        if (token_tf_val != NULL) {
            // printf("token TF val: %lf\n", *token_tf_val);
            // printf("token IDF val: %lf\n", _node_to_tidf_val(token)->idf_val);
            file_tf_idf_val += ((*token_tf_val) * _node_to_tidf_val(token)->idf_val);
        }
    }

    printf("TF_IDF val = %.12lf\n", file_tf_idf_val);
}

LinkedList *calculate_tf_idf(HashTable *tf_index, LinkedList *token_idf_list) {
    LinkedList *file_tf_idf_list = ll_init();

    ht_entry_map(tf_index, _tf_idf_file_map, token_idf_list, file_tf_idf_list);

    return file_tf_idf_list;
}