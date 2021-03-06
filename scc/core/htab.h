#ifndef HTAB_H
#define HTAB_H

#include "list.h"
#include "alloc.h"
#include "error.h"

struct htab
{
        struct list_node* buckets;
        size_t size;
        size_t elems;
        struct allocator* alloc;
};

#define htab_init_alloc(size, allocator) { NULL, size, 0, allocator }
#define htab_init(size) htab_init_alloc(size, STD_ALLOC)

extern void      htab_initf(struct htab* htab, size_t size, struct allocator* alloc);
extern void      htab_delete(struct htab* htab);
extern scc_err_t htab_insert(struct htab* htab, uint64_t key, void* val);
extern int       htab_exists(struct htab* htab, uint64_t key);
extern void*     htab_get(struct htab* htab, uint64_t key);

#endif // !HTAB_H
