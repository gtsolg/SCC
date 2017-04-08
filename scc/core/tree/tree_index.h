#ifndef TREE_INDEX_H
#define TREE_INDEX_H

#include "tree.h"

struct tree_index
{
        struct htab nodes;
};

extern void tree_index_initf(struct tree_index* index);
extern void tree_index_delete(struct tree_index* index);

extern void tree_index_add(struct tree_index* index, tree node, uint64_t key);
extern tree tree_index_get(struct tree_index* index, uint64_t key);
extern int  tree_index_node_exists(struct tree_index* index, uint64_t key);

#endif // !TREE_INDEX_H
