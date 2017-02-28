#ifndef C_TREE_H
#define C_TREE_H

#include "tree.h"
#include "ctoken.h"

extern tree c_tree_create_const(const struct c_token* cst_token);
extern tree c_tree_exp_create(enum operator_kind op, unsigned nesting);

#endif // !C_TREE_H
