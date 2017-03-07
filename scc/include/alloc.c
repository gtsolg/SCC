#include "alloc.h"

static void* def_alloc(void* ptr, size_t size)
{
        return malloc(size);
}

static void def_dealloc(void* ptr, void* block)
{
        return free(block);
}

struct allocator __std_alloc = allocator_init(NULL, def_alloc, def_dealloc);
