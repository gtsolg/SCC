#include "tree_index.h"

extern void tree_index_initf(struct tree_index* index)
{
        htab_initf(&index->nodes, 10, STD_ALLOC);
}

extern void tree_index_delete(struct tree_index* index)
{
        htab_delete(&index->nodes);
}

extern void tree_index_add(struct tree_index* index, tree node, uint64_t key)
{
        htab_insert(&index->nodes, key, node);
}

extern tree tree_index_get(struct tree_index* index, uint64_t key)
{
        return htab_get(&index->nodes, key);
}

extern int tree_index_node_exists(struct tree_index* index, uint64_t key)
{
        return (int)tree_index_get(index, key);
}
