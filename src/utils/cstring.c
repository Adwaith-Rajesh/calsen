#include "cstring.h"

#include <stdio.h>
#include <stdlib.h>

String *string_create(size_t size) {
    if (size < 1) return NULL;
    String *new_string;

    new_string = malloc(sizeof(String));
    new_string->str = malloc(sizeof(char) * size);

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

void string_append_charp(String *string, char *charp) {
    size_t i = 0;
    while (charp[i] != '\0') {
        string_append_char(string, charp[i++]);
    }
}

void string_print(String *string) {
    if (string == NULL) return;
    printf("%s", string->str);
}

String *string_create_from_charp(char *str, size_t size) {
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
    // we kind of have to remove the null byte that was prev set
    // might hinder with other standard lib function.
    // so fo now the best approach is to just replace it with some random character
    string->str[string->curr_p] = '$';
    string->curr_p = 0;
    string->str[0] = '\0';
}
