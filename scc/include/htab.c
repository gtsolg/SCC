#include "htab.h"

struct entry
{
        struct list_node node;
        uint64_t key;
        void*    val;
};

static inline struct entry* create_entry(alloc_fn alloc, uint64_t key, void* val)
{
        struct entry* entry = alloc(sizeof(*entry));
        if (!entry)
                return NULL;
        entry->node = list_null_node;
        entry->key = key;
        entry->val = val;
        return entry;
}

extern void htab_delete(struct htab* htab)
{
        for (size_t i = 0; i < htab->size; i++)
                while (1)
                {
                        struct entry* entry = list_pop_back(htab->buckets + i);
                        if (!entry)
                                break;
                        htab->alloc.deallocate(entry);
                }
        htab->alloc.deallocate(htab->buckets);
        htab->buckets = NULL;
        htab->size = 0;
}

static inline struct list_node* create_buckets(alloc_fn alloc, size_t size)
{
        struct list_node* buckets = alloc(sizeof(struct list_node) * size);
        for (size_t i = 0; i < size; i++)
                list_init(buckets + i);
        return buckets;
}

static inline void htab_grow(struct htab* htab)
{
        size_t new_size = 1 + htab->size * 2;
        struct list_node* new_buckets = create_buckets(htab->alloc.allocate, new_size);

        for (size_t i = 0; i < htab->size; i++)
                while (1)
                {
                        struct entry* entry = list_pop_back(htab->buckets + i);
                        if (!entry)
                                break;
                        list_push_back(new_buckets + entry->key % new_size, entry);
                }
        htab_delete(htab);
        htab->buckets = new_buckets;
        htab->size = new_size;
}

extern int htab_insert(struct htab* htab, uint64_t key, void* val)
{
        if (!htab->buckets)
                htab->buckets = create_buckets(htab->alloc.allocate, htab->size);

        if (htab->size == htab->elems)
                htab_grow(htab);

        struct entry* entry = create_entry(htab->alloc.allocate, key, val);
        if (!entry)
                return 0;
        list_push_back(&htab->buckets[key % htab->size], entry);
        htab->elems++;
        return 1;
}

extern int htab_exists(struct htab* htab, uint64_t key)
{
        return htab_get(htab, key) != NULL;
}

extern void* htab_get(struct htab* htab, uint64_t key)
{
        if (!htab->buckets)
                return NULL;

        struct list_node* list = htab->buckets + key % htab->size;
        struct entry* it = list_head(list);
        while (it != list_end(list) && it->key != key)
                it = list_node_next(&it->node);

        return it == list_end(list) ? NULL : it->val;
}