#include "tf_idf.h"

#include <stdio.h>
#include <stdlib.h>

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
HashTable *calculate_tf(LinkedList *token_list) {
    HashTable *ht = ht_create();
    LLIter *iter = ll_iter_init(token_list);

    void *next_val = ll_iter_next(iter);
    while (next_val != NULL) {
        // check if it exists in the hash table
        void *entry = ht_get(ht, ((String *)next_val)->str);
        if (entry == NULL) {
            _allocate_and_init(ht, ((String *)next_val)->str);
            next_val = ll_iter_next(iter);
            continue;
        }
        (*((int *)entry))++;  // increment the existing value
        next_val = ll_iter_next(iter);
    }

    ll_iter_free(iter);
    return ht;
}

void tf_table_free_int(void *int_val) {
    if (int_val == NULL) return;
    free(int_val);
}