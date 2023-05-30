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

HashTable *token_count(LinkedList *token_list);
void calculate_tf(HashTable *tf_values, int token_count);
void tf_table_free_int(void *int_val);
double calculate_idf(HashTable *index_table, String *token);

TokenIDFVal *create_token_idf_val(String *token, double idf_val);
FileTFIDFVal *create_file_tf_idf_val(const char *filename, double tf_idf_val);
void free_token_idf_val(TokenIDFVal *tf_idf_val);
void free_file_tf_idf_val(FileTFIDFVal *ft_idf_val);
LinkedList *calculate_tf_idf(HashTable *tf_index, LinkedList *token_idf_list);
LinkedList *filter_sort_file_tf_idf_list(LinkedList *file_tf_idf_list, int n_results);

#endif
