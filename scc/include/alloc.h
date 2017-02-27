#ifndef ALLOC_H
#define ALLOC_H

#include "scc.h"

typedef void*(*alloc_fn)(size_t);
typedef void(*dealloc_fn)(void*);

struct allocator
{
        alloc_fn   allocate;
        dealloc_fn deallocate;
};

#define allocator_init(alloc, dealloc) { alloc, dealloc }
#define default_alloc() allocator_init(malloc, free)

#endif // !ALLOC_H
