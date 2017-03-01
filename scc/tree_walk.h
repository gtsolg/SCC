#ifndef TREE_WALK_H
#define TREE_WALK_H

#include "tree.h"

typedef void(*tree_walk_fn)(tree node, void*);
typedef int(*tree_match_fn)(tree node, void*);

extern void tree_walk_list(tree list, tree_walk_fn walk, void* info);

#endif // !TREE_WALK_H
