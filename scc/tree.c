#include "tree.h"
#include "scc.h"

extern tree tree_create(struct allocator* alloc, enum tree_node_kind kind)
{
        tree node = allocate(alloc, sizeof(*node));
        if (!node)
                return NULL;
        tree_kind(node) = kind;
        return node;
}

extern void tree_delete(struct allocator* alloc, tree node)
{
        deallocate(alloc, node);
}

extern tree tree_copy(struct allocator* alloc, tree node)
{
        tree t = allocate(alloc, sizeof(*node));
        if (!t)
                return NULL;
        *t = *node;
        return t;
}

extern tree tree_type_create(struct allocator* alloc, enum type_kind kind, enum type_qualifier qual, tree type)
{
        tree t = tree_create(alloc, tnk_type);
        if (!t)
                return NULL;
        tree_type_kind(t) = kind;
        tree_type_qual(t) = qual;
        tree_type(t) = type;
        return t;
}

extern tree tree_ident_create(struct allocator* alloc, strref_t ref)
{
        tree id = tree_create(alloc, tnk_id);
        if (!id)
                return NULL;
        tree_id_ref(id) = ref;
        return id;
}

extern tree tree_list_node_create(struct allocator* alloc, tree base)
{
        tree node = tree_create(alloc, tnk_list_node);
        if (!node)
                return NULL;
        tree_list_node(node) = list_null_node;
        tree_list_node_base(node) = base;
        return node;
}

extern tree tree_exp_create(struct allocator* alloc, enum operator_kind kind, unsigned nesting, const struct tree_exp_info* opinfo)
{
        tree exp = tree_create(alloc, tnk_exp);
        if (!exp)
                return NULL;
        tree_exp_prev(exp) = NULL;
        tree_exp_left(exp) = NULL;
        tree_exp_right(exp) = NULL;
        tree_exp_info(exp) = opinfo[kind];
        tree_exp_nesting(exp) = nesting;
        return exp;
}

extern tree tree_const_int_create(struct allocator* alloc, uint64_t val)
{
        tree cst = tree_create(alloc, tnk_const_int);
        if (!cst)
                return NULL;
        tree_const_int(cst) = val;
        return cst;
}

extern tree tree_const_real_create(struct allocator* alloc, long double val)
{
        tree cst = tree_create(alloc, tnk_const_real);
        if (!cst)
                return NULL;
        tree_const_real(cst) = val;
        return cst;
}

extern tree tree_const_float_create(struct allocator* alloc, float val)
{
        tree cst = tree_create(alloc, tnk_const_float);
        if (!cst)
                return NULL;
        tree_const_float(cst) = val;
        return cst;
}

extern tree tree_const_string_create(struct allocator* alloc, strref_t ref)
{
        tree cst = tree_create(alloc, tnk_const_string);
        if (!cst)
                return NULL;
        tree_const_strref(cst) = ref;
        return cst;
}

extern tree tree_attrib_create(struct allocator* alloc, uint64_t att)
{
        tree node = tree_create(alloc, tnk_attrib);
        if (!node)
                return NULL;
        tree_attrib(node) = att;
        return node;
}

extern tree tree_vector_type_create(struct allocator* alloc, tree type, uint64_t size)
{
        tree vec = tree_create(alloc, tnk_vector_type);
        if (!vec)
                return NULL;
        tree_vector_type(vec) = type;
        tree_vector_size(vec) = size;
        return vec;
}