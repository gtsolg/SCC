#ifndef POOL_H
#define POOL_H

#include "list.h"

struct pool
{
        void* obstack;
        const size_t obsize;
        size_t size;
};

#define pool_init(obj_size, size) { NULL, obj_size, size }

static inline void __pool_grow(struct pool* pool)
{
        size_t obj_to_add = 1 + pool->size;
        size_t real_size = sizeof(void*) + pool->obsize;
        pool->size += obj_to_add;

        void* objects = malloc(real_size * obj_to_add);
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
}

static inline void* pool_get(struct pool* pool)
{
        if (!pool->obstack)
                __pool_grow(pool);

        char* obj = pool->obstack;
        pool->obstack = *(void**)obj;
        return obj + sizeof(void*);
}

static inline void pool_return(struct pool* pool, void* obj)
{
        (char*)obj -= sizeof(void*);
        *(void**)obj = pool->obstack;
        pool->obstack = obj;
}

#endif // !POOL_H
