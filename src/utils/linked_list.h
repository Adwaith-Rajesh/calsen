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

Node *create_node(void *data);
void free_node(Node *node);

LinkedList *ll_init();
void ll_print(LinkedList *list, NodePrinterFn *node_printer);
int ll_is_empty(LinkedList *list);
void ll_append(LinkedList *list, Node *node);
Node *ll_pop(LinkedList *list);
void ll_append_left(LinkedList *list, Node *node);
Node *ll_pop_left(LinkedList *list);
void ll_map(LinkedList *list, LinkedListMapFn *fn, ...);
void ll_empty(LinkedList *list);
void ll_free(LinkedList *list);

LLIter *ll_iter_init(LinkedList *list);
void ll_iter_free(LLIter *iterator);
Node *ll_iter_next(LLIter *iterator);

#endif
