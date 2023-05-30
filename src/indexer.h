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

FileWithMIME *create_file_with_mime(String *filename, String *mime_type);
void free_file_with_mime(FileWithMIME *fwm);

void get_files_to_index(char *dirpath, LinkedList *list);

HashTable *load_index(const char *index_file);
void dump_index(const char *filepath, HashTable *table);

#endif
