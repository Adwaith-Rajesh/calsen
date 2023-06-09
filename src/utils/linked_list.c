#include "linked_list.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "memfns.h"

Node *create_node(void *data) {
    Node *new_node = (Node *)malloc_with_check(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_node(Node *node) {
    if (node == NULL) return;
    free(node);
}

LinkedList *ll_init() {
    LinkedList *new_list = (LinkedList *)malloc_with_check(sizeof(LinkedList));

    new_list->head = NULL;
    new_list->size = 0;
    return new_list;
}

void ll_print(LinkedList *list, NodePrinterFn *node_printer) {
    Node *temp = list->head;
    while (temp != NULL) {
        node_printer(temp);
        temp = temp->next;
    }
}

int ll_is_empty(LinkedList *list) {
    return list->size == 0;
}

void ll_append(LinkedList *list, Node *node) {
    if (list->head == NULL) {
        list->head = node;
        list->size++;
        return;
    }

    Node *temp = list->head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = node;
    list->size++;
}

Node *ll_pop(LinkedList *list) {
    if (list->size == 1) {
        Node *temp = list->head;
        list->head = NULL;
        list->size--;
        return temp;
    }
    Node *prev = NULL;
    Node *curr = list->head;

    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = NULL;
    list->size--;
    return curr;
}

void ll_append_left(LinkedList *list, Node *node) {
    if (list->head == NULL) {
        list->head = node;
        list->size++;
        return;
    }
    node->next = list->head;
    list->head = node;
    list->size++;
}

Node *ll_pop_left(LinkedList *list) {
    if (list->head == NULL) return NULL;
    Node *temp = list->head;
    list->head = list->head->next;
    list->size--;
    return temp;
}

void ll_empty(LinkedList *list) {
    Node *temp;
    while (list->head != NULL) {
        temp = list->head;
        list->head = list->head->next;
        free_node(temp);
    }
    list->head = NULL;
    list->size = 0;
}

void ll_free(LinkedList *list) {
    Node *temp;
    while (list->head != NULL) {
        temp = list->head;
        list->head = list->head->next;
        free_node(temp);
    }
    free(list);
}

LLIter *ll_iter_init(LinkedList *list) {
    LLIter *new_iter = (LLIter *)malloc_with_check(sizeof(LLIter));
    new_iter->curr = list->head;
    return new_iter;
}

Node *ll_iter_next(LLIter *iterator) {
    if (iterator->curr == NULL) return NULL;
    Node *curr_val = iterator->curr;
    iterator->curr = iterator->curr->next;
    return curr_val;
}

void ll_iter_free(LLIter *iterator) {
    if (iterator == NULL) return;
    free(iterator);
}

void ll_map(LinkedList *list, LinkedListMapFn *fn, ...) {
    Node *temp = list->head;
    va_list args;
    while (temp != NULL) {
        va_start(args, fn);
        temp->data = fn(temp, args);
        temp = temp->next;
    }
    va_end(args);
}
