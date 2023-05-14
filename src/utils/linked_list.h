#ifndef LIBS_INCLUDE_LINKED_LIST_H
#define LIBS_INCLUDE_LINKED_LIST_H

#include <stdarg.h>
#include <stdio.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} LinkedList;

typedef struct {
    Node *curr;
} LLIter;

typedef void NodePrinterFn(Node *);
typedef void *LinkedListMapFn(Node *, va_list);

Node *create_node(void *);
void free_node(Node *);

LinkedList *ll_init();
void ll_print(LinkedList *, NodePrinterFn *);
int ll_is_empty(LinkedList *);
void ll_append(LinkedList *, Node *);
Node *ll_pop(LinkedList *);
void ll_append_left(LinkedList *, Node *);
Node *ll_pop_left(LinkedList *);
void ll_map(LinkedList *, LinkedListMapFn *, ...);
void ll_empty(LinkedList *);
void ll_free(LinkedList *);

LLIter *ll_iter_init(LinkedList *);
void ll_iter_free(LLIter *);
Node *ll_iter_next(LLIter *);

#endif
