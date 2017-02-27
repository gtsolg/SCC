#ifndef HASH_H
#define HASH_H

#include "scc.h"

static inline uint64_t hash64_str_len(const char* str, size_t len)
{
        const uint64_t m = 0xc6a4a7935bd1e995;
        const int r = 47;

        uint64_t h = m;
        for (size_t i = 0; i < len; i++)
        {
                uint64_t k = str[i] * m;
                k ^= k >> r;
                k *= m;
                h ^= k;
                h *= m;
        }
        h += h * (uint64_t)(1 << len);
        switch (len & 7)
        {
                case 7: h ^= (uint64_t)str[6] << 48;
                case 6: h ^= (uint64_t)str[5] << 40;
                case 5: h ^= (uint64_t)str[4] << 32;
                case 4: h ^= (uint64_t)str[3] << 24;
                case 3: h ^= (uint64_t)str[2] << 16;
                case 2: h ^= (uint64_t)str[1] << 8;
                case 1: h ^= (uint64_t)str[0];
                        h *= m;
        }
        h ^= h >> r;
        h *= m;
        h ^= h >> r;
        return h;
}

static inline uint64_t hash64_str(const char* str)
{
        return hash64_str_len(str, strlen(str));
}

static inline uint64_t hash64_ptr(void* ptr)
{
        uint64_t h = 0xac88ab93c832a;
        h *= (uint64_t)ptr;
        h ^= !(uint64_t)ptr;
        return h;
}

#endif // HASH_H