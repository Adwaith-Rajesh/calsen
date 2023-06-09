#ifndef _INCLUDE_HASH_TABLE
#define _INCLUDE_HASH_TABLE

#define HASH_TABLE_SIZE 2500

#include <stdarg.h>
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
typedef void HTValuePrintFn(void *);
typedef void HtEntryMapFn(HTEntry *, va_list);

HashTable *ht_create(void);
void ht_set(HashTable *table, const char *key, void *value);
void *ht_get(HashTable *table, const char *key);
void ht_free(HashTable *table);
// allows you to free the keys before the table itself get freed
void ht_free_map(HashTable *table, HtFreeMapFn *fn);
void ht_print(HashTable *table, HTValuePrintFn *fn);
void ht_entry_map(HashTable *table, HtEntryMapFn *fn, ...);
size_t ht_get_size(HashTable *table);
#endif
