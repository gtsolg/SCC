#ifndef POOL_H
#define POOL_H

#include "alloc.h"
#include "list.h"

struct pool
{
        void*  obstack;
        size_t obsize;
        size_t size;
        struct allocator* alloc;
        int block_idx;
        void* blocks[64];
};

#define pool_init_alloc(obj_size, size, alloc) { NULL, obj_size, size, alloc, 0 }
#define pool_init(obj_size, size) pool_init_alloc(obj_size, size, STD_ALLOC)

static inline void pool_initf(struct pool* pool, size_t obsize, size_t size, struct allocator* alloc)
{
        pool->obstack = NULL;
        pool->obsize = obsize;
        pool->size = size;
        pool->alloc = alloc;
        pool->block_idx = 0;
}

static inline scc_err_t __pool_grow(struct pool* pool)
{
        size_t obj_to_add = 1 + pool->size;
        size_t real_size = sizeof(void*) + pool->obsize;
        pool->size += obj_to_add;

        void* objects = allocate(pool->alloc, real_size * obj_to_add);
        if (!objects)
                return SCC_ERR;

        pool->blocks[pool->block_idx++] = objects;

        size_t i = 0;
        for (; i < obj_to_add - 1; i++)
        {
                void* obj = (char*)objects + i * real_size;
                void* next = (char*)obj + real_size;
                *(void**)obj = next;
        }
        void* last = (char*)objects + real_size * i;
        *(void**)last = NULL;
        pool->obstack = objects;
        return SCC_SUCCESS;
}

static inline void pool_delete(struct pool* pool)
{
        for (int i = pool->block_idx - 1; i > -1; i--)
                deallocate(pool->alloc, pool->blocks[i]);
        pool->block_idx = 0;
        pool->size = 0;
        pool->obstack = NULL;
}

static inline void* pool_get(struct pool* pool)
{
        if (!pool->obstack)
                if (__pool_grow(pool))
                        return NULL;

        char* obj = pool->obstack;
        pool->obstack = *(void**)obj;
        return obj + sizeof(void*);
}

static inline void pool_return(struct pool* pool, void* obj)
{
        if (!obj)
                return;
        (char*)obj -= sizeof(void*);
        *(void**)obj = pool->obstack;
        pool->obstack = obj;
}

#endif // !POOL_H
