#include "tree_pass.h"

extern void tree_walk_list(tree list, tree_pass_fn pass, void* info)
{
        tree it = tree_list_iterator_init(list);
        while (tree_list_iterator_valid(it))
        {
                pass(it, tree_list_node_base(it), info);
                tree_list_iterator_advance(it);
        }
}
