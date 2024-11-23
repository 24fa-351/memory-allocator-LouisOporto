#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>
#include <pthread.h>

typedef char ALIGN[16]; // 16-byte alignment

// Define memory block structure
union header {
    struct {
        size_t size;
        unsigned is_free;
        union header* next;
    } s;
    ALIGN stub;
};
typedef union header header_t;

// Define memory interface
void *modMalloc(size_t size);  // malloc reset allocated blocks to 0

void modFree(void* ptr);

void* modRealloc(void* ptr, size_t size);  // Less focus on this for now

#endif