#ifndef STR_POOL_H
#define STR_POOL_H

#include "alloc.h"
#include "htab.h"

typedef uint64_t strref_t;

struct str_pool
{
        struct htab set;
        struct allocator* stralloc;
};

#define str_pool_init_alloc(size, halloc, salloc) { htab_init_alloc(size, halloc), salloc }
#define str_pool_init(size) str_pool_init_alloc(size, STD_ALLOC, STD_ALLOC)

extern struct str_pool __std_pool;

#define STD_STR_POOL &__std_pool

extern strref_t    string_pool_compute_ref(const char* string, size_t len);
extern void        string_pool_reset(struct str_pool* pool);
extern const char* string_pool_get_string_by_ref(struct str_pool* pool, strref_t ref);
extern strref_t    string_pool(struct str_pool* pool, const char* string, size_t len);
extern int         string_pooled(struct str_pool* pool, strref_t ref);

#endif // !STR_POOL_H
