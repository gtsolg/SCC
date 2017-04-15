#ifndef TREE_ITERATOR_H
#define TREE_ITERATOR_H

#include "tree_node.h"

struct tree_iterator
{
        tree pos;
        int kind;
        void* data;
};

#define __tree_it(ptree) ((struct tree_iterator*)ptree)

#define tree_iterator_init(pos, kind, data) { pos, kind, data }
#define tree_iterator_pos(ptree)       __tree_it(ptree)->pos
#define tree_iterator_node_kind(ptree) tree_kind(tree_iterator_pos(ptree))
#define tree_iterator_kind(ptree)      __tree_it(ptree)->kind
#define tree_iterator_data(ptree)      __tree_it(ptree)->data

#define tree_exp_iterator_is(ptree, kind)  tree_exp_is(tree_iterator_pos(ptree), kind)
#define tree_exp_iterator_go_left(ptree)  (tree_iterator_pos(ptree) = tree_exp_left(tree_iterator_pos(ptree)))
#define tree_exp_iterator_go_right(ptree) (tree_iterator_pos(ptree) = tree_exp_right(tree_iterator_pos(ptree)))
#define tree_exp_iterator_go_prev(ptree)  (tree_iterator_pos(ptree) = tree_exp_prev(tree_iterator_pos(ptree)))
#define tree_exp_iterator_valid(ptree)    (tree_is(tree_iterator_pos(ptree), tnk_exp))

#define tree_list_forward_iterator_init(plist) tree_iterator_init(tree_list_head(plist), -1, NULL)
#define tree_list_reverse_iterator_init(plist) tree_iterator_init(tree_list_tail(plist), -1, NULL)
#define tree_list_iterator_advance(ptree)     (tree_iterator_pos(ptree) = tree_list_node_next(tree_iterator_pos(ptree)))
#define tree_list_iterator_rewind(ptree)      (tree_iterator_pos(ptree) = tree_list_node_prev(tree_iterator_pos(ptree)))
#define tree_list_iterator_valid(ptree)       (tree_iterator_node_kind(ptree) == tnk_list_node)
#define tree_list_iterator_node_base(ptree)    tree_list_node_base(tree_iterator_pos(ptree))

#endif // !TREE_ITERATOR_H
