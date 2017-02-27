#ifndef C_TREE_H
#define C_TREE_H

#include "tree.h"
#include "ctoken.h"

extern tree c_tree_create_const(const struct c_token* cst_token);
extern tree c_null_exp_node_create();
extern tree c_exp_list_node_create(enum operator_kind op, unsigned nesting);
extern tree c_unexp_list_node_create(enum operator_kind op, unsigned nesting, tree next);

#endif // !C_TREE_H
