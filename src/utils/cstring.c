#include "cstring.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memfns.h"

String *string_create(size_t size) {
    if (size < 1) return NULL;
    String *new_string;

    new_string = malloc_with_check(sizeof(String));
    new_string->str = malloc_with_check(sizeof(char) * size);

    new_string->str[0] = '\0';
    new_string->curr_p = 0;
    new_string->size = size;
    return new_string;
}

void string_destroy(String *string) {
    if (string == NULL) return;
    if (string->str != NULL) {
        free(string->str);
    }
    free(string);
}

void string_append_char(String *string, char ch) {
    if (string->curr_p < string->size - 1) {
        string->str[string->curr_p] = ch;
        string->str[++string->curr_p] = '\0';
        return;
    }
}

void string_append_charp(String *string, const char *charp) {
    size_t i = 0;
    while (charp[i] != '\0') {
        string_append_char(string, charp[i++]);
    }
}

void string_print(String *string) {
    if (string == NULL) return;
    printf("%s", string->str);
}

String *string_create_from_charp(const char *str, size_t size) {
    String *new_string = string_create(size + 1);
    size_t i = 0;
    while (str[i] != '\0') {
        string_append_char(new_string, str[i++]);
    }
    return new_string;
}

String *string_create_from_charp_slice(CharPSlice *slice) {
    String *new_str = string_create(slice->size + 1);
    for (size_t i = 0; i < slice->size; i++) {
        string_append_char(new_str, slice->data[i]);
    }
    return new_str;
}

void charp_slice_to_charp_array(CharPSlice *slice, char arr[]) {
    size_t i = 0;
    for (; i < slice->size; i++) {
        arr[i] = slice->data[i];
    }
    arr[i] = '\0';
}

void charp_slice_print(CharPSlice *slice) {
    char temp_arr[slice->size + 1];
    charp_slice_to_charp_array(slice, temp_arr);
    printf("%s", temp_arr);
}

void string_reset(String *string) {
    memset(string->str, 0, string->size);
    string->str[0] = '\0';
    string->curr_p = 0;
}

// check if the char can be appended, otherwise create a new string
// with double the size. It' static as indexer might be the only place where this is needed
String *string_expandable_append(String *str, char c) {
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

String *string_strip(String *string, int destroy_prev) {
    String *new_string = string_create(string->size);

    if (string->size == 0) {
        if (destroy_prev == 1) string_destroy(string);
        return new_string;  // we always return a new string
    }

    size_t start_idx = 0;
    size_t end_idx = string->curr_p - 1;
    while (start_idx < string->size && isspace(string->str[start_idx])) start_idx++;  // removes the leading whitespaces
    while (end_idx >= start_idx && isspace(string->str[end_idx])) end_idx--;

    for (size_t i = start_idx; i <= end_idx; i++) {
        string_append_char(new_string, string->str[i]);
    }

    if (destroy_prev == 1) string_destroy(string);
    return new_string;
}
