#ifndef _INCLUDE_HASH_TABLE
#define _INCLUDE_HASH_TABLE

#define HASH_TABLE_SIZE 2500

#include <stdlib.h>

typedef struct pair {
    char *key;
    void *value;
    struct pair *next;
} HTEntry;

typedef struct {
    HTEntry **entries;
} HashTable;

typedef void HtFreeMapFn(void *);

HashTable *ht_create(void);
void ht_set(HashTable *, const char *, void *);
void *ht_get(HashTable *, const char *);
void ht_free(HashTable *);
// allows you to free the keys before the table itself get freed
void ht_free_map(HashTable *, HtFreeMapFn *);

#endif
