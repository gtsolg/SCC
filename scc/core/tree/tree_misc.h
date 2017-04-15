#ifndef TREE_MISC_H
#define TREE_MISC_H

#include "tree_node.h"

static union tree_node __null_node;
#define TREE_NULL &__null_node

extern tree tree_create(struct allocator* alloc, enum tree_node_kind kind);
extern tree tree_copy(struct allocator* alloc, tree node);
extern void tree_delete(struct allocator* alloc, tree node);
extern void tree_delete_recursive(struct allocator* alloc, tree node);

extern tree tree_exp_stmt_create(struct allocator* alloc, tree decls, tree base);
extern tree tree_type_create(struct allocator* alloc, enum type_kind kind, enum type_qualifier qual, tree type);
extern tree tree_ident_create(struct allocator* alloc, strref_t ref);
extern tree tree_var_decl_create(struct allocator* alloc, tree var_name, tree base);
extern tree tree_list_node_create(struct allocator* alloc, tree base);
extern tree tree_exp_create(struct allocator* alloc, enum operator_kind kind, unsigned nesting, const struct tree_exp_info* opinfo);

extern tree tree_list_create(struct allocator* alloc);
extern void tree_list_nodes_delete(struct allocator* alloc, tree list);
extern void tree_list_delete(struct allocator* alloc, tree list);
extern union tree_node tree_list_initf(tree list);

extern tree tree_const_int_create(struct allocator* alloc, uint64_t val);
extern tree tree_const_real_create(struct allocator* alloc, long double val);
extern tree tree_const_float_create(struct allocator* alloc, float val);
extern tree tree_const_string_create(struct allocator* alloc, strref_t ref);
extern tree tree_attrib_create(struct allocator* alloc, uint64_t att);
extern tree tree_vector_type_create(struct allocator* alloc, tree type, uint64_t size);
extern tree tree_sign_type_create(struct allocator* alloc, tree res, tree args);

static inline void tree_swap(tree* a, tree* b)
{
        tree t = *a;
        *a = *b;
        *b = t;
}

#endif // !TREE_ALLOC_H
