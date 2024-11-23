#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "malloc.h"

pthread_mutex_t global_malloc_lock;
header_t *head;
header_t *tail;

header_t *get_free_block(size_t size) {
    header_t *curr = head;
    while(curr) {
        if(curr->s.is_free && curr->s.size >= size) {
            return curr;
        }
        curr = curr->s.next;
    }
    return NULL;
}

// Define memory interface
void* modMalloc(size_t size) {
    size_t total_size;
    void *block;
    header_t *header;
    if(!size) { return NULL; }

    pthread_mutex_lock(&global_malloc_lock);
    // Find a free block before allocating a new one
    header = get_free_block(size);
    if(header) {
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header + 1);
    }
    total_size = sizeof(header_t) + size;
    block = sbrk(total_size); // sbrk() is a system call

    if(block == (void*) -1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }
    header = block;
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = NULL;
    if(!head) {
        head = header;
    }
    if(tail) {
        tail->s.next = header;
    }
    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void*)(header + 1);
    block = sbrk(size);
}



void* modRealloc(void* ptr, size_t size) {
    void* newPtr = realloc(ptr, size);  // Implement this
    if (newPtr == NULL) {
        fprintf(stderr, __FILE__ ":%d realloc failed\n", __LINE__);
        exit(1);
    }
    fprintf(stderr, __FILE__ ":%d: realloc(%p, %lu) = %p\n", __LINE__, ptr,
            size, newPtr);
    return newPtr;
}

void modFree(void* ptr) {
    header_t *header;
    header_t *tmp;

    void *programbreak;

    if (!ptr) return;

    pthread_mutex_lock(&global_malloc_lock);
    header = (header_t*)ptr - 1;

    programbreak = sbrk(0);
    if ((char*)ptr + header->s.size == programbreak) {
        if (head == tail) {
            head = tail = NULL;
        } else {
            tmp = head;
            while (tmp) {
                if (tmp->s.next == tail) {
                    tmp->s.next = NULL;
                    tail = tmp;
                }
                tmp = tmp->s.next;
            }
        }
        sbrk(0 - sizeof(header_t) - header->s.size);
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }
    header->s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}
