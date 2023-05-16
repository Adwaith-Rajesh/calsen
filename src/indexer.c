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

#include <dirent.h>
#include <linux/limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "cstring.h"
#include "hash_table.h"
#include "linked_list.h"

/*
Recursively traverse / walk a directory, then the full path along the files MIME type
stored in the linked list

NOTE:
get_file_to_index, avoids the usage of nftw FTW(3), as there are no easy ways to pass custom
arguments to the function called by it [at least none that I know of] (we need to add things to a linked list). other than
making the required variable global, which is a complete NO NO from me.
*/
void get_files_to_index(const char *dirpath, LinkedList *list) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dirpath)) == NULL) return;
    while ((entry = readdir(dir)) != NULL) {
        // TODO: complete this: Good night
    }
}

/*

The hash table that is loaded or the table that is generated
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
    // the max size fo a filepath in linux is PATH_MAX(4096) +
    va_list args_copy;
    va_copy(args_copy, args);

    FILE *fp = va_arg(args_copy, FILE *);
    String *line = string_create(PATH_MAX);

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
