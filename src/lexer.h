#ifndef _INCLUDE_LEXER_H
#define _INCLUDE_LEXER_H

#include <stdlib.h>

#include "cstring.h"
#include "linked_list.h"

typedef struct {
    char *content;
    size_t curr_idx;
} Lexer;

LinkedList *file_content_to_tokens(char *content, size_t size, int *token_count);

#endif
