#include "map.h"

static inline void dealloc_node(struct list_node* node, dealloc_t dealloc)
{
        if (node)
        {
                dealloc(node->next);
                dealloc(node->prev);
                dealloc(node);
        }
}

extern void map_destroy(struct map* map, dealloc_t dealloc)
{
        dealloc(map->root);
}

static inline int height(struct list_node* node)
{
        if (node)
        {
                int prev = height(node->prev);
                int next = height(node->next);
                return (next > prev ? next : prev) + 1;
        }
        return 0;
}

static inline int balance_factor(struct list_node* node)
{
        return height(node->prev) - height(node->next);
}

static inline struct list_node* rrot(struct list_node* parent)
{
        struct list_node* next = parent->next;
        parent->next = next->prev;
        next->prev = parent;
        return next;
}

static inline struct list_node* lrot(struct list_node* parent)
{
        struct list_node* prev = parent->prev;
        parent->prev = prev->next;
        prev->next = parent;
        return prev;
}

static inline struct list_node* lrrot(struct list_node* parent)
{
        struct list_node* prev = parent->prev;
        parent->prev = rrot(prev);
        return lrot(parent);
}

static inline struct list_node* rlrot(struct list_node* parent)
{
        struct list_node* next = parent->next;
        parent->next = rrot(next);
        return rrot(parent);
}

static inline struct list_node* balance(struct list_node* node)
{
        int balance = balance_factor(node);
        if (balance > 1)
        {
                if (balance_factor(node->prev) > 0)
                        node = lrot(node);
                else
                        node = lrrot(node);
        }
        else if (balance < -1)
        {
                if (balance_factor(node->next) > 0)
                        node = rlrot(node);
                else
                        node = rrot(node);
        }
        return node;
}
//static inline struct map_node* insert(struct map_node*)

//extern void map_insert(struct map* map, struct list_node* node)
//{
//        if (!map->root)
//        {
//                map->root = node;
//                return map->root;
//        }
//}

extern int map_exists(struct map* map, cmp_t cmp, void* key)
{

}

extern struct list_node* map_find(struct map* map, cmp_t cmp, void* key)
{

}