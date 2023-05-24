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
    va_list args_copy;
    va_copy(args_copy, args);
    size_t table_size = va_arg(args_copy, size_t);

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
    va_list args_copy;
    va_copy(args_copy, args);

    int *t_count = va_arg(args_copy, int *);
    String *token = va_arg(args_copy, String *);

    if (strcmp(entry->key, token->str) == 0) *t_count = *t_count + 1;
}

static void _tf_table_iter(HTEntry *entry, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    int *t_count = va_arg(args_copy, int *);
    String *token = va_arg(args_copy, String *);

    ht_entry_map(entry->value, _tf_table_iter_inner, t_count, token);
}

// index_table -> the entire index
double calculate_idf(HashTable *index_table, String *token) {
    size_t n_docs = ht_get_size(index_table);

    // count the number of files the token exist in
    int t_count = 0;
    ht_entry_map(index_table, _tf_table_iter, &t_count, token);

    printf("docs count: %ld\n", n_docs);
    printf("token count: %d\n", t_count);

    return log10((size_t)t_count / t_count);
}