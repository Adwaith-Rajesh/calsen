#include "lexer.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

#include "cstring.h"
#include "linked_list.h"

/*

"""
there was a fox that could not jump. Ha Ha

1 : 23
"""
    |
    v
there, was, a, fox, that, could, not, jump, ., Ha, Ha, 1, :, 23
*/

/*
This function is as function to map overt the token list
*/
static void *_to_lowercase(Node *node, va_list arg_list) {
    (*(va_arg(arg_list, int *)))++;  // inc the total count
    if (node == NULL) return NULL;
    String *str = (String *)(node->data);
    int i = 0;
    while (str->str[i] != '\0') {
        str->str[i] = tolower(str->str[i]);
        i++;
    }
    return node->data;
}

typedef int TokenCreateCond(int);

static CharPSlice _create_next_token(Lexer *lexer, TokenCreateCond *cond) {
    int curr_c = (int)(lexer->content[lexer->curr_idx]);  // current char
    CharPSlice slice = {
        .data = lexer->content + lexer->curr_idx,
        .size = 0,
    };

    while (cond(curr_c)) {
        slice.size++;
        curr_c = lexer->content[++lexer->curr_idx];
    }
    return slice;
}

static CharPSlice _get_next_token(Lexer *lexer, size_t content_size) {
    while (lexer->curr_idx < content_size) {
        int curr_c = (int)(lexer->content[lexer->curr_idx]);
        if (isdigit(curr_c)) {
            return _create_next_token(lexer, isdigit);
        } else if (isalpha(curr_c)) {
            return _create_next_token(lexer, isalpha);
        }
        // it's okay to void punctuation marks as it introduces unwanted noise.
        // might add it back if found necessary.
        lexer->curr_idx++;
    }
    return (CharPSlice){.data = NULL, .size = 0};
}

/*
The token_count is necessary as lexing is the only time we know total number of
tokens parses. i.e the number of token before they were converted to lowercase.
This is necessary as calculating the tf requires the original token count
*/
LinkedList *file_content_to_tokens(char *content, size_t size, int *token_count) {
    Lexer lexer = {
        .content = content,
        .curr_idx = 0,
    };

    LinkedList *token_list = ll_init();

    while (lexer.curr_idx < size) {
        CharPSlice t = _get_next_token(&lexer, size);

        if ((t.data == NULL) && (t.size == 0)) break;
        ll_append_left(token_list,
                       create_node(string_create_from_charp_slice(&t)));
    }
    ll_map(token_list, _to_lowercase, token_count);
    return token_list;
}
