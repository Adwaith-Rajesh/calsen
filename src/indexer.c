// The indexer deals with loading and dumping the indexed data

/*
The indexed data is stored flat file

| --- filenames begin with a colon.
|     this can be guaranteed as the lexer ignores punctuation marks.
v

:filename=/absolute/path/to/file
token1=0.3
token2=0.3
token3=0.3
:filename=/absolute/path/to/other/file
token1=0.444
token1=0.444
token1=0.444
*/
#include "indexer.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"
#include "hash_table.h"
#include "linked_list.h"
LinkedList *get_file_to_index(const char *dirpath) {}

/*

The hash table that is loaded or the table that is generated is generated
will be of the following type

{
    filename: {
        token1: 0.3444,
        token3: 0.444
    },

    filename2: {
        token1: 0.44,
        token3: 0.444
    }
}
// the above representation is for reference purpose only, (Things are not exactly like
this in memory XD)

So basically we will have a HashTable* with each entry where a
key, "points" to another hash table where each key is token with its tf value

*/

/* =============== Load the contents from the file =============== */

HashTable *load_index(const char *index_file) {
}

/* =============== Dump the contents to a file =============== */

static inline void _dump_line(FILE *fp, char *line, size_t size) {
    fwrite(line, size, 1, fp);
}

static void _dump_index_d_as_val(HTEntry *entry, va_list args) {
    // takes care of writing the individual tokens to the indexer file
    if (entry == NULL) return;
    va_list args_copy;
    va_copy(args_copy, args);

    FILE *fp = va_arg(args_copy, FILE *);
    String *line = va_arg(args_copy, String *);

    string_append_charp(line, entry->key);
    string_append_char(line, '=');
    _dump_line(fp, line->str, line->curr_p);
    fprintf(fp, "%.12lf\n", *((double *)(entry->value)));
    string_reset(line);
}

static void _dump_index_map_ht_as_val(HTEntry *entry, va_list args) {
    // this dump function will take case writing the file name to the indexer file

    if (entry == NULL) return;
    // the max size fo a filepath in linux is 256 +
    // assumption that the max word length on a file would be 100, 512 is a nice line size
    va_list args_copy;
    va_copy(args_copy, args);

    printf("one entry\n");

    FILE *fp = va_arg(args_copy, FILE *);
    String *line = string_create(512);

    string_append_charp(line, ":filename=");
    string_append_charp(line, entry->key);
    string_append_char(line, '\n');
    _dump_line(fp, line->str, line->curr_p);
    string_reset(line);

    // write the values of the tokens to the file
    ht_entry_map((HashTable *)entry->value, _dump_index_d_as_val, fp, line);

    string_destroy(line);
}

void dump_index(const char *filepath, HashTable *table) {
    FILE *fp = fopen(filepath, "w");
    fseek(fp, 0, SEEK_SET);
    ht_entry_map(table, _dump_index_map_ht_as_val, fp);
    fclose(fp);
}
