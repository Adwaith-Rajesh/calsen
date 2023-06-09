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

#endif
