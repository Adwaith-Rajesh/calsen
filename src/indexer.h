#ifndef _INCLUDE_INDEX_H
#define _INCLUDE_INDEX_H

#include "cstring.h"
#include "hash_table.h"
#include "linked_list.h"

/*
Assuming the worst case scenario every time, that the file path will be PATH_MAX,
having the struct own the filename, will consume a lot of unnecessary memory.
type of filepath is String*
*/
typedef struct {
    String *filepath;
    String *mime_type;
} FileWithMIME;

void get_files_to_index(const char *, LinkedList *list);

HashTable *load_index(const char *);
void dump_index(const char *, HashTable *);

#endif
