#ifndef _INCLUDE_CSTRING_H
#define _INCLUDE_CSTRING_H

#include <stddef.h>

#include "arena.h"

typedef struct {
    char *str;
    size_t size;
    size_t curr_p;
} String;

String *string_create(size_t size, Arena *arena);
void string_destroy(String *string);
void string_append_char(String *string, char ch);
void string_print(String *string);
String *string_create_from_charp(char *str, size_t size, Arena *arena);

#endif
