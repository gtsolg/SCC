#ifndef LIST_H
#define LIST_H

#include "scc.h"

struct list_node
{
        struct list_node* next;
        struct list_node* prev;
};

#define list_node_init() { NULL, NULL }
#define list_node_next(nodeptr) ((nodeptr)->next)
#define list_node_prev(nodeptr) ((nodeptr)->prev)

#define list_end(listptr)   (listptr)
#define list_head(listptr)  list_node_next(listptr)
#define list_tail(listptr)  list_node_prev(listptr)
#define list_empty(listptr) (list_head(listptr) == listptr)

static inline void list_initf(struct list_node* list)
{
        list->next = list;
        list->prev = list;
}

static const struct list_node list_null_node = list_node_init();

static inline void list_add_after(struct list_node* node, struct list_node* new_node)
{
        struct list_node* next = node->next;
        if (next)
                next->prev = new_node;
        new_node->next = next;
        new_node->prev = node;
        node->next = new_node;
}

static inline void list_add_before(struct list_node* node, struct list_node* new_node)
{
        struct list_node* prev = node->prev;
        if (prev)
                prev->next = new_node;
        new_node->prev = prev;
        new_node->next = node;
        node->prev = new_node;
}

static inline void list_push_front(struct list_node* list, struct list_node* node)
{
        list_add_after(list, node);
        if (list_empty(list))
        {
                list->prev = node;
                node->next = list;
        }
}

static inline void list_push_back(struct list_node* list, struct list_node* node)
{
        list_add_before(list, node);
        if (list_empty(list))
        {
                list->next = node;
                node->prev = list;
        }
}

static inline void list_push_front_list(struct list_node* to, struct list_node* from)
{
        if (list_empty(to))
                *to = *from;
        else
        {
                list_head(to)->prev = list_tail(from);
                list_tail(from)->next = list_head(to);
                list_head(from)->prev = to;
                to->next = list_head(from);
        }
        list_initf(from);
}

static inline void list_push_back_list(struct list_node* to, struct list_node* from)
{
        if (list_empty(to))
                *to = *from;
        else
        {
                list_tail(to)->next = list_head(from);
                list_head(from)->prev = list_tail(to);
                list_tail(from)->next = to;
                to->prev = list_tail(from);
        }
        list_initf(from);
}

static inline struct list_node* list_remove(struct list_node* node)
{
        node->prev->next = node->next;
        node->next->prev = node->prev;
        return node;
}

static inline struct list_node* list_pop_back(struct list_node* list)
{
        return list_remove(list_tail(list));
}

static inline struct list_node* list_pop_front(struct list_node* list)
{
        return list_remove(list_head(list));
}

#define list_pop_front_off(listptr, s, m)\
        ((char*)list_pop_front(listptr) - offsetof(s, m))

#define list_pop_back_off(listptr, s, m)\
        ((char*)list_pop_back(listptr) - offsetof(s, m))

#define list_head_off(listptr, s, m)\
        (list_head(listptr) ? (char*)list_head(listptr) - offsetof(s, m) : NULL)

#define list_tail_off(listptr, s, m)\
        (list_tail(listptr) ? (char*)list_tail(listptr) - offsetof(s, m) : NULL)

#define list_next_off(nodeptr, s, m) list_head_off(nodeptr, s, m)
#define list_prev_off(nodeptr, s, m) list_tail_off(nodeptr, s, m)

#endif // !LIST_H
