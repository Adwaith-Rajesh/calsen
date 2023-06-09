#ifndef _INCLUDE_CSTRING_H
#define _INCLUDE_CSTRING_H

#include <stddef.h>

typedef struct {
    char *data;
    size_t size;
} CharPSlice;

typedef struct {
    char *str;
    size_t size;
    size_t curr_p;
} String;

String *string_create(size_t size);
void string_destroy(String *string);
void string_append_char(String *string, char ch);
void string_append_charp(String *string, const char *charp);
void string_print(String *string);
String *string_create_from_charp(const char *str, size_t size);
String *string_create_from_charp_slice(CharPSlice *slice);
void charp_slice_to_charp_array(CharPSlice *slice, char arr[]);
void charp_slice_print(CharPSlice *slice);
void string_reset(String *string);
String *string_expandable_append(String *str, char c);

// strip String of trailing white space characters and returns a new
// String which has to be freed
// if destroy_prev is true / 1 then the provided string is destroyed.
String *string_strip(String *string, int destroy_prev);

#endif
