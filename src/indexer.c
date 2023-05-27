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
#include <magic.h>  // sudo apt install libmagic-dev
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "cstring.h"
#include "hash_table.h"
#include "linked_list.h"
#include "path.h"

// =============== Getting the files ===============

static String *_get_mime_type(char *filename) {
    magic_t magic;
    const char *mime_type;

    magic = magic_open(MAGIC_MIME_TYPE);
    magic_load(magic, NULL);
    mime_type = magic_file(magic, filename);

    String *mime_string = string_create_from_charp(mime_type, strlen(mime_type));
    magic_close(magic);
    return mime_string;
}

FileWithMIME *create_file_with_mime(String *filename, String *mime_type) {
    FileWithMIME *fwm = malloc(sizeof(FileWithMIME));
    if (fwm == NULL) return NULL;
    fwm->filepath = filename;
    fwm->mime_type = mime_type;
    return fwm;
}

void free_file_with_mime(FileWithMIME *fwm) {
    if (fwm == NULL) return;
    string_destroy(fwm->filepath);
    string_destroy(fwm->mime_type);
    free(fwm);
}

/*
Recursively traverse / walk a directory, then the full path along the files MIME type
stored in the linked list

NOTE:
get_file_to_index, avoids the usage of nftw FTW(3), as there are no easy ways to pass custom
arguments to the function called by it [at least none that I know of] (we need to add things to a linked list). other than
making the required variable global, which is a complete NO NO from me.

Returns:
    LinkedList[FileWithMIME *];
*/
void get_files_to_index(char *dirpath, LinkedList *list) {
    char resolved_path[PATH_MAX];
    char *dir_full_path = get_absolute_path(dirpath, resolved_path);

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dir_full_path)) == NULL) return;
    while ((entry = readdir(dir)) != NULL) {
        char full_file_path[PATH_MAX];
        snprintf(full_file_path, PATH_MAX, "%s/%s", dir_full_path, entry->d_name);

        if (is_dir(full_file_path)) {
            if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) continue;
            get_files_to_index(full_file_path, list);
        } else {
            // we get a file, append the filename with its MIME type to the list
            ll_append_left(list,
                           create_node(create_file_with_mime(
                               string_create_from_charp(full_file_path, strlen(full_file_path)),
                               _get_mime_type(full_file_path))));
        }
    }
    closedir(dir);
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

// check if the char can be appended, otherwise create a new string
// with double the size. It' static as indexer might be the only place where this is needed
static String *_string_expandable_append(String *str, char c) {
    if (str->curr_p < str->size - 1) {
        string_append_char(str, c);
        return str;
    } else {
        String *new_string = string_create_from_charp(str->str, str->size * 2);
        string_destroy(str);
        return new_string;
    }
    return NULL;
}

static HashTable *_read_token(FILE *fp) {
    HashTable *token_table = ht_create();
    int ch;
    char *tw;

    // until the next ':' appears all the tokens belong the the current file
    while ((ch = fgetc(fp)) != ':' && ch != EOF) {
        // read the token name
        String *token_line = string_create(20);
        token_line = _string_expandable_append(token_line, ch);
        while ((ch = fgetc(fp)) != '=' && ch != EOF) {
            token_line = _string_expandable_append(token_line, ch);
        }

        // read the TF val (14 bytes, after the '=')
        char tf_val[15];
        fgets(tf_val, 15, fp);

        double *db_val = malloc(sizeof(double));
        *db_val = strtod(tf_val, &tw);
        ht_set(token_table, token_line->str, db_val);

        fgetc(fp);  // ignore the newline
    }
    fseek(fp, -1, SEEK_CUR);  // so that next fgetc should get the ':'
    return token_table;
}

// read the line character by character, we won't be using getline as
// the length of a line can vary according to the size of the token parsed
// thus we read character by character until we reach a new line or EOF
HashTable *load_index(const char *index_file) {
    FILE *fp = fopen(index_file, "r");
    String *line = string_create(30);
    HashTable *index_table = ht_create();
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        // move the seek after the '=' (filename begins)
        if (ch == ':') {
            fseek(fp, 9, SEEK_CUR);
            // now we read the filename
            while ((ch = fgetc(fp)) != EOF && ch != '\n') {
                line = _string_expandable_append(line, ch);
            }
            ht_set(index_table, line->str, NULL);

            // read the tokens
            ht_set(index_table, line->str, _read_token(fp));
            string_reset(line);
        }
    }

    string_destroy(line);
    fclose(fp);

    return index_table;
}

/* =============== Dump the contents to a file =============== */

static inline void _dump_line(FILE *fp, char *line, size_t size) {
    fwrite(line, size, 1, fp);
}

static void _dump_index_d_as_val(HTEntry *entry, va_list args) {
    // takes care of writing the individual tokens to the indexer file
    if (entry == NULL) return;

    FILE *fp = va_arg(args, FILE *);
    String *line = va_arg(args, String *);

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

    FILE *fp = va_arg(args, FILE *);
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
