#ifndef _INCLUDE_HASH_TABLE
#define _INCLUDE_HASH_TABLE

#define HASH_TABLE_SIZE 1000

#include <stdlib.h>

typedef struct pair {
    char *key;
    void *value;
    struct pair *next;
} HTEntry;

typedef struct {
    HTEntry **entries;
} HashTable;

HashTable *ht_create(void);
void ht_set(HashTable *, const char *, void *);
void *ht_get(HashTable *, const char *);
short int ht_key_exist(HashTable *, const char *);
void ht_free(HashTable *);

#endif
