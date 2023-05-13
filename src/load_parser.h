#ifndef _INCLUDE_LOAD_PARSER_H
#define _INCLUDE_LOAD_PARSER_H

#include "hash_table.h"

typedef String *ParseFileFn(const char *);

HashTable *load_all_parsers();
ParseFileFn *load_parser_entry_point(HashTable *, char *);
void unload_parser(void *);

#endif
