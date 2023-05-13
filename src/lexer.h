#ifndef _INCLUDE_LEXER_H
#define _INCLUDE_LEXER_H

#include <stdlib.h>

#include "arena.h"
#include "linked_list.h"

LinkedList *file_content_to_tokens(char *, size_t, Arena *);

#endif
