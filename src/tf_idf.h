#ifndef _INCLUDE_TF_IDF_H
#define _INCLUDE_TF_IDF_H

#include "cstring.h"
#include "hash_table.h"
#include "linked_list.h"

HashTable *token_count(LinkedList *);
void calculate_tf(HashTable *, int);
void tf_table_free_int(void *);
double calculate_idf(HashTable *, String *);

#endif