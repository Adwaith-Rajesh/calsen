#ifndef _INCLUDE_LEXER_H
#define _INCLUDE_LEXER_H

#include <stdlib.h>

#include "cstring.h"
#include "linked_list.h"

typedef enum {
    STRING,
    NUMBER,
    PUNCT,
    UNKNOWN
} token_t;

typedef struct {
    CharPSlice data;
    token_t token;
} Token;

typedef struct {
    char *content;
    size_t curr_idx;
} Lexer;

LinkedList *file_content_to_tokens(char *, size_t);

#endif
