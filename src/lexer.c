#include "lexer.h"

#include <ctype.h>
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

typedef int TokenCreateCond(int);

static const char *_get_token_name(token_t token_type) {
    switch (token_type) {
        case STRING:
            return "STRING";
        case NUMBER:
            return "NUMBER";
        case PUNCT:
            return "PUNCT";
        default:
            return "UNKNOWN";
    }
}

static void _print_token(Token *t) {
    printf("%s: ", _get_token_name(t->token));
    charp_slice_print(&(t->data));
    printf("\n");
}

static Token _create_next_token(Lexer *lexer, TokenCreateCond *cond, token_t token_type) {
    int curr_c = (int)(lexer->content[lexer->curr_idx]);
    Token token = {
        .data = {.data = lexer->content + lexer->curr_idx, .size = 0},
        .token = token_type,
    };

    while (cond(curr_c)) {
        token.data.size++;
        curr_c = lexer->content[++lexer->curr_idx];
    }
    return token;
}

static Token _get_next_token(Lexer *lexer, size_t content_size) {
    while (lexer->curr_idx < content_size) {
        int curr_c = (int)(lexer->content[lexer->curr_idx]);
        if (isdigit(curr_c)) {
            return _create_next_token(lexer, isdigit, NUMBER);
        } else if (isalpha(curr_c)) {
            return _create_next_token(lexer, isalpha, STRING);
        } else if (ispunct(curr_c)) {
            return _create_next_token(lexer, ispunct, PUNCT);
        }

        lexer->curr_idx++;
    }
    return (Token){{.data = NULL, .size = 0}, .token = UNKNOWN};
}

LinkedList *file_content_to_tokens(char *content, size_t size) {
    Lexer lexer = {
        .content = content,
        .curr_idx = 0,
    };

    LinkedList *token_list = ll_init();

    while (lexer.curr_idx < size) {
        Token t = _get_next_token(&lexer, size);

        if (t.token == UNKNOWN && t.data.data == NULL) break;
        ll_append(token_list,
                  create_node(string_create_from_charp_slice(&(t.data))));
        // TODO: remove stmt
        _print_token(&t);
    }
    return token_list;
}
