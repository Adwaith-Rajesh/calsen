#ifndef _INCLUDE_ARENA_H
#define _INCLUDE_ARENA_H

#include <stddef.h>

#define REGION_DEFAULT_CAP (8 * 1024)  // 8Kib

typedef struct region {
    size_t size;
    size_t cap;
    void *mem;
    struct region *next;
} Region;

typedef struct {
    Region *reg;
} Arena;

Arena *arena_init();
void *arena_alloc(Arena *, size_t);
void arena_free(Arena *);

#endif
