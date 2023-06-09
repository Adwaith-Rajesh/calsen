#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memfns.h"

static unsigned int _hash(const char *key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }
    value = value % HASH_TABLE_SIZE;
    return value;
}

static HTEntry *ht_pair(const char *key, void *value) {
    HTEntry *new_entry = malloc_with_check(sizeof(HTEntry));
    new_entry->key = malloc_with_check(strlen(key) + 1);
    new_entry->value = value;

    strcpy(new_entry->key, key);
    new_entry->next = NULL;
    return new_entry;
}

HashTable *ht_create(void) {
    HashTable *new_ht = malloc_with_check(sizeof(HashTable));
    new_ht->entries = malloc_with_check(sizeof(HTEntry *) * HASH_TABLE_SIZE);

    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        new_ht->entries[i] = NULL;
    }
    return new_ht;
}

void ht_set(HashTable *table, const char *key, void *value) {
    unsigned int slot = _hash(key);
    HTEntry *entry = table->entries[slot];

    if (entry == NULL) {
        table->entries[slot] = ht_pair(key, value);
        return;
    }

    HTEntry *prev;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            // replace the data
            entry->value = value;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
    // end of chain without a match, create a new pair
    prev->next = ht_pair(key, value);
}

void *ht_get(HashTable *table, const char *key) {
    unsigned int slot = _hash(key);
    HTEntry *entry = table->entries[slot];

    if (entry == NULL) return NULL;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) return entry->value;
        entry = entry->next;
    }
    return NULL;
}

void ht_free(HashTable *table) {
    if (table == NULL) return;

    for (size_t i = 0; i < HASH_TABLE_SIZE; ++i) {
        HTEntry *entry = table->entries[i];

        if (entry == NULL) continue;

        HTEntry *temp;
        while (entry != NULL) {
            free(entry->key);
            temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(table->entries);
    free(table);
}

void ht_free_map(HashTable *table, HtFreeMapFn *fn) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HTEntry *entry = table->entries[i];
        if (entry != NULL) {
            while (entry != NULL) {
                fn(entry->value);
                entry = entry->next;
            }
        }
    }
}

void ht_print(HashTable *table, HTValuePrintFn *fn) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HTEntry *entry = table->entries[i];
        while (entry != NULL) {
            printf("(%s, ", entry->key);
            fn(entry->value);
            puts(")");
            entry = entry->next;
        }
    }
}

/*
Get the raw entry from the Hash Table and then modify it however you want.
If the next field is modified, unexpected behavior can occur
*/
void ht_entry_map(HashTable *table, HtEntryMapFn *fn, ...) {
    va_list args;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HTEntry *entry = table->entries[i];
        while (entry != NULL) {
            va_start(args, fn);
            fn(entry, args);
            entry = entry->next;
        }
    }
    va_end(args);
}

size_t ht_get_size(HashTable *table) {
    size_t size = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HTEntry *entry = table->entries[i];
        while (entry != NULL) {
            size++;
            entry = entry->next;
        }
    }
    return size;
}
