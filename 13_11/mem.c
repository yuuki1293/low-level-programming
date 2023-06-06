#include "mem.h"

#define BLOCK_MIN_SIZE 1024

/**
 * @brief queryにヘッダサイズを加算する。
 *
 * @param query query size
 */
static void expand_query(size_t* query) {
    *query += sizeof(struct mem);
}

static bool is_available(size_t query, struct mem* memory) {
    return (*memory).is_free
        && query <= (*memory).capacity;
}

static void* map_heap(void* addr, size_t len) {
    return mmap(addr, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
}

static void* map_new(struct mem* memory, size_t query) {
    if (query < BLOCK_MIN_SIZE)
        query = BLOCK_MIN_SIZE;

    memory = map_heap(memory, query);
    return memory;
}

static void* map_new_next(struct mem* memory, size_t query) {
    return map_new(memory + (*memory).capacity, query);
}

static bool is_connectable(struct mem* front, struct mem* back) {
    return front < back && back->is_free;
}

void* _malloc(size_t query) {
    struct mem* memory, * prev = HEAP_START;
    expand_query(&query);

    for (memory = (*prev).next;;prev = memory, memory = (*memory).next) {
        if (is_available(query, memory)) {
            (*memory).capacity -= query;
            prev = (*memory).next;
            (*memory).next = (struct mem*)((char*)memory + (*memory).capacity);
            memory = (*memory).next;
            (*memory).next = prev;
            (*memory).capacity = query;
            (*memory).is_free = false;
            break;
        }

        if (memory == HEAP_START) {
            memory = map_new_next(memory, query);
            (*memory).next = (*prev).next;
            (*prev).next = memory;
            (*memory).capacity = query;
            (*memory).is_free = true;
        }
    }

    return (char*)memory + sizeof(struct mem);
}

void  _free(void* mem) {
    struct mem* header = (struct mem*)((char*)mem - sizeof(struct mem));
    header->is_free = true;
    if (is_connectable(header, header->next)) {
        header->capacity += header->next->capacity;
        header->next = header->next->next;
    }
}

void* heap_init(size_t initial_size) {
    struct mem* memory;
    expand_query(&initial_size);
    if (initial_size < BLOCK_MIN_SIZE)
        initial_size = BLOCK_MIN_SIZE;
    memory = map_heap(HEAP_START, initial_size);
    (*memory).capacity = initial_size;
    (*memory).is_free = true;
    (*memory).next = memory;

    return HEAP_START;
}