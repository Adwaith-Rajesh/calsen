#ifndef _INCLUDE_TF_IDF_H
#define _INCLUDE_TF_IDF_H

#include "cstring.h"
#include "hash_table.h"
#include "linked_list.h"

typedef struct {
    // this is not an owned String, points to a String that already exists
    String *token;
    double idf_val;
} TokenIDFVal;

typedef struct {
    // this is an owned String, must be freed while freeing the struct
    String *filename;
    double tf_idf_val;
} FileTFIDFVal;

HashTable *token_count(LinkedList *);
void calculate_tf(HashTable *, int);
void tf_table_free_int(void *);
double calculate_idf(HashTable *, String *);

TokenIDFVal *create_token_idf_val(String *, double);
FileTFIDFVal *create_file_tf_idf_val(String *, double);
void free_token_idf_val(TokenIDFVal *);
void free_file_tf_idf_val(FileTFIDFVal *);

#endif
