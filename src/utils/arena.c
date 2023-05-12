// simple arena allocator

#include "arena.h"

#include <stdlib.h>

static Region *_create_region(size_t size) {
    Region *new_region = malloc(sizeof(Region));
    new_region->size = 0;
    size_t capacity = (size > REGION_DEFAULT_CAP) ? size : REGION_DEFAULT_CAP;
    new_region->cap = capacity;
    new_region->mem = malloc(capacity);
    new_region->next = NULL;
    return new_region;
}

static void _free_region(Region *r) {
    if (r == NULL) return;
    if (r->mem != NULL) {
        free(r->mem);
    }
    free(r);
}

Arena *arena_init() {
    Arena *new_arena = malloc(sizeof(Arena));
    new_arena->reg = NULL;
    return new_arena;
}

void *arena_alloc(Arena *arena, size_t size) {
    if (arena->reg == NULL) {
        Region *new_region = _create_region(size);
        arena->reg = new_region;
        new_region->size = size;
        return arena->reg->mem + 0;
    }

    Region *temp = arena->reg;
    Region *prev;
    while (temp != NULL) {
        if ((temp->cap - temp->size) >= size) {
            void *loc = temp->mem + temp->size;
            temp->size += size;
            return loc;
        }
        prev = temp;
        temp = temp->next;
    }

    // reached the end, couldn't find a region to allocate we create a new region
    prev->next = _create_region(size);
    void *loc = prev->mem + 0;
    prev->size += size;
    return loc;
}

void arena_free(Arena *arena) {
    Region *temp;
    while (arena->reg != NULL) {
        temp = arena->reg;
        arena->reg = arena->reg->next;
        _free_region(temp);
    }
    free(arena);
}
