#include "str_pool.h"

#include "hash.h"
#include "str.h"

struct str_pool __std_pool = str_pool_init(100);

extern strref_t string_pool_compute_ref(const char* string, size_t len)
{
        return hash64_str_len(string, len);
}

extern void string_pool_reset(struct str_pool* pool)
{
        // todo
}

extern const char* string_pool_get_string_by_ref(struct str_pool* pool, strref_t ref)
{
        return htab_get(&pool->set, ref);
}

extern strref_t string_pool(struct str_pool* pool, const char* string, size_t len)
{
        strref_t ref = string_pool_compute_ref(string, len);
        if (string_pooled(pool, ref))
                return ref;
        char* copy = allocate(pool->stralloc, len + 1);
        copy[len] = '\0';
        memcpy(copy, string, len);
        htab_insert(&pool->set, ref, copy);
        return ref;
}

extern int string_pooled(struct str_pool* pool, strref_t ref)
{
        return htab_exists(&pool->set, ref);
}
