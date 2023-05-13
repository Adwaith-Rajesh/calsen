#include "cstring.h"

#include <stdio.h>
#include <stdlib.h>

#include "arena.h"

String *string_create(size_t size, Arena *arena) {
    if (size < 1) return NULL;
    String *new_string;
    if (arena == NULL) {
        new_string = malloc(sizeof(String));
        new_string->str = malloc(sizeof(char) * size);
    } else {
        new_string = arena_alloc(arena, sizeof(String));
        new_string->str = arena_alloc(arena, sizeof(char) * size);
    }

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

String *string_create_from_charp(char *str, size_t size, Arena *arena) {
    String *new_string = string_create(size + 1, arena);
    size_t i = 0;
    while (str[i] != '\0') {
        string_append_char(new_string, str[i++]);
    }
    return new_string;
}
