#ifndef _INCLUDE_TF_IDF_H
#define _INCLUDE_TF_IDF_H

#include "hash_table.h"
#include "linked_list.h"

HashTable *calculate_tf(LinkedList *);
void tf_table_free_int(void *);

#endif