#ifndef MAP_H
#define MAP_H

#include "scc.h"
#include "alloc.h"
#include "list.h"

// To use map you'll have to implement your own insert and search functions.
// 
// struct sample_node
// {
//         struct list_node node;
//         int val;
// };
// 
// struct sample_node* sample_insert(struct sample_node* root, struct sample_node* node)
// {
//         if (!root)
//                 root = &node->node;
//         else if (root->val < node->val)
//                 root->node.prev = sample_insert(root->node.prev, node);
//         else
//                 root->node.next = sample_insert(root->node.next, node);
//         return map_balance(root);
// }

struct map
{
        struct list_node* root;
};

#define map_init() { NULL }

static inline int map_height(struct list_node* node)
{
        if (node)
        {
                int prev = map_height(node->prev);
                int next = map_height(node->next);
                return (next > prev ? next : prev) + 1;
        }
        return 0;
}

static inline int map_balance_factor(struct list_node* node)
{
        return map_height(node->prev) - map_height(node->next);
}

static inline struct list_node* map_rrot(struct list_node* parent)
{
        struct list_node* next = parent->next;
        parent->next = next->prev;
        next->prev = parent;
        return next;
}

static inline struct list_node* map_lrot(struct list_node* parent)
{
        struct list_node* prev = parent->prev;
        parent->prev = prev->next;
        prev->next = parent;
        return prev;
}

static inline struct list_node* map_lrrot(struct list_node* parent)
{
        struct list_node* prev = parent->prev;
        parent->prev = map_rrot(prev);
        return map_lrot(parent);
}

static inline struct list_node* map_rlrot(struct list_node* parent)
{
        struct list_node* next = parent->next;
        parent->next = map_rrot(next);
        return map_rrot(parent);
}

static inline struct list_node* map_balance(struct list_node* node)
{
        int balance = map_balance_factor(node);
        if (balance > 1)
        {
                if (map_balance_factor(node->prev) > 0)
                        node = map_lrot(node);
                else
                        node = map_lrrot(node);
        }
        else if (balance < -1)
        {
                if (map_balance_factor(node->next) > 0)
                        node = map_rlrot(node);
                else
                        node = map_rrot(node);
        }
        return node;
}

#endif // !MAP_H