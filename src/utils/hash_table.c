#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    HTEntry *new_entry = malloc(sizeof(HTEntry));
    new_entry->key = malloc(strlen(key) + 1);
    new_entry->value = value;

    strcpy(new_entry->key, key);
    new_entry->next = NULL;
    return new_entry;
}

HashTable *ht_create(void) {
    HashTable *new_ht = malloc(sizeof(HashTable));
    new_ht->entries = malloc(sizeof(HTEntry *) * HASH_TABLE_SIZE);

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
