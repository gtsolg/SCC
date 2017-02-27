#include "str_intern.h"

#include "htab.h"
#include "hash.h"

struct htab str_pool = htab_init(100);

extern const char* str_intern(const char* str)
{
        uint64_t h = hash64_str(str);
        if (str_interned(str))
                return htab_get(&str_pool, h);

        char* copy = malloc(strlen(str) + 1);
        strcpy(copy, str);
        htab_insert(&str_pool, h, copy);
        return copy;
}

extern const char* str_intern_from(const char* pos, size_t len)
{
        uint64_t h = hash64_str_len(pos, len);
        if (htab_exists(&str_pool, h))
                return htab_get(&str_pool, h);

        char* copy = malloc(len + 1);
        memcpy(copy, pos, len);
        copy[len] = '\0';
        htab_insert(&str_pool, h, copy);
        return copy;
}

extern int str_interned(const char* str)
{
        return htab_exists(&str_pool, hash64_str(str));
}