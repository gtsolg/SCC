#ifndef TREE_PASS_H
#define TREE_PASS_H

#include "tree.h"

typedef void(*tree_pass_fn)(tree node, void*);

typedef uint64_t tree_pass_prefs;

#define TREE_PASS_NONE 0

// allocates its own stack of nodes
extern void tree_foreach_alloc(struct allocator* tree_alloc, tree root, tree_pass_fn pass, void* data, tree_pass_prefs prefs);

#endif // !TREE_PASS_H
