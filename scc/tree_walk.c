#include "tree_walk.h"

extern void tree_walk_list(tree list, tree_walk_fn walk, void* info)
{
        tree it = tree_list_iterator_init(list);
        while (tree_list_iterator_valid(it))
        {
                walk(it, tree_list_node_base(it), info);
                tree_list_iterator_advance(it);
        }
}
