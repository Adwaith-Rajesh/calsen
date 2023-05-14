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

String *string_create(size_t);
void string_destroy(String *);
void string_append_char(String *, char);
void string_append_charp(String *, char *);
void string_print(String *);
String *string_create_from_charp(char *, size_t);
String *string_create_from_charp_slice(CharPSlice *);
void charp_slice_to_charp_array(CharPSlice *, char[]);
void charp_slice_print(CharPSlice *);

#endif
