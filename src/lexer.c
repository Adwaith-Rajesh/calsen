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
    (void)arg_list;
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
        } else if (ispunct(curr_c)) {
            return _create_next_token(lexer, ispunct);
        }

        lexer->curr_idx++;
    }
    return (CharPSlice){.data = NULL, .size = 0};
}

LinkedList *file_content_to_tokens(char *content, size_t size) {
    Lexer lexer = {
        .content = content,
        .curr_idx = 0,
    };

    LinkedList *token_list = ll_init();

    while (lexer.curr_idx < size) {
        CharPSlice t = _get_next_token(&lexer, size);

        if ((t.data == NULL) && (t.size = 0)) break;
        ll_append_left(token_list,
                       create_node(string_create_from_charp_slice(&t)));
    }
    ll_map(token_list, _to_lowercase);
    return token_list;
}
