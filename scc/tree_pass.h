#ifndef TREE_WALK_H
#define TREE_WALK_H

#include "tree.h"

typedef void(*tree_pass_fn)(tree node, void*);
typedef int(*tree_match_fn)(tree node, void*);

extern void tree_pass_list(tree list, tree_pass_fn pass, void* info);

#endif // !TREE_WALK_H
