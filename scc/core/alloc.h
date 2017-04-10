#ifndef ALLOC_H
#define ALLOC_H

#include "scc.h"

typedef void*(*alloc_fn)(void*, size_t);
typedef void(*dealloc_fn)(void*, void*);

struct allocator
{
        void*      pool;
        alloc_fn   alloc;
        dealloc_fn dealloc;
};

extern struct allocator __std_alloc;
#define STD_ALLOC &__std_alloc

// todo std reentrant alloc
// extern sturct allocator __std_alloc_r;
// #define STD_ALLOC_R

#define allocator_init(pool_ptr, alloc, dealloc) { pool_ptr, alloc, dealloc }

#define alloc_ptr(palloc) ((struct allocator*)palloc)
#define allocate(palloc, size)  (alloc_ptr(palloc)->alloc(alloc_ptr(palloc)->pool, (size)))
#define deallocate(palloc, ptr) (alloc_ptr(palloc)->dealloc(alloc_ptr(palloc)->pool, (ptr)))

static inline void alloc_initf(struct allocator* alloc, void* pool, alloc_fn a, dealloc_fn d)
{
        alloc->pool = pool;
        alloc->alloc = a;
        alloc->dealloc = d;
}

#endif // !ALLOC_H
