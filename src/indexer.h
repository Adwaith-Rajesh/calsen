#ifndef _INCLUDE_INDEX_H
#define _INCLUDE_INDEX_H

#include "cstring.h"
#include "hash_table.h"
#include "linked_list.h"

LinkedList *get_file_to_index(const char *);

HashTable *load_index(const char *);
void dump_index(const char *, HashTable *);

#endif
