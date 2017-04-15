#include "tree.h"

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
        if (node && !tree_is(node, tnk_null))
                deallocate(alloc, node);
}

static int tree_delete_pass(tree node, void* alloc)
{
        tree_delete(alloc, node);
        return PASS_SHOULD_CONTINUE;
}

extern void tree_delete_recursive(struct allocator* alloc, tree node)
{
        if (!node)
                return;
        tree_foreach(alloc, node,
                tree_pass_initf(tree_delete_pass, alloc, PASS_PREFS_NONE),
                tree_match_everything);
}

extern void tree_list_nodes_delete(struct allocator* alloc, tree list)
{
        if (!list)
                return;
        struct tree_iterator it = tree_list_forward_iterator_init(list);
        while (tree_list_iterator_valid(&it))
        {
                tree node = tree_iterator_pos(&it);
                tree_list_iterator_advance(&it);
                tree_delete(alloc, node);
        }
}

extern void tree_list_delete(struct allocator* alloc, tree list)
{
        if (!list)
                return;
        tree_list_nodes_delete(alloc, list);
        tree_delete(alloc, list);
}

extern tree tree_copy(struct allocator* alloc, tree node)
{
        tree t = allocate(alloc, sizeof(*node));
        if (!t)
                return NULL;
        *t = *node;
        return t;
}

extern tree tree_exp_stmt_create(struct allocator* alloc, tree decls, tree base)
{
        tree s = tree_create(alloc, tnk_stmt);
        if (!s)
                return NULL;
        tree_stmt_kind(s) = sk_exp;
        tree_exp_stmt_decls(s) = decls;
        tree_exp_stmt_base(s) = base;
        return s;
}

extern tree tree_type_create(struct allocator* alloc, enum type_kind kind, enum type_qualifier qual, tree type)
{
        tree t = tree_create(alloc, tnk_type);
        if (!t)
                return NULL;
        tree_type_kind(t) = kind;
        tree_type_qual(t) = qual;
        tree_type_next(t) = type;
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

extern tree tree_var_decl_create(struct allocator* alloc, tree var_name, tree base)
{
        tree decl = tree_create(alloc, tnk_type_decl);
        if (!decl)
                return NULL;
        tree_decl_id(decl) = var_name;
        tree_decl_base(decl) = base;
        return decl;
}

extern tree tree_list_node_create(struct allocator* alloc, tree base)
{
        tree node = tree_create(alloc, tnk_list_node);
        if (!node)
                return NULL;
        __tree_list_node(node) = list_null_node;
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

extern tree tree_list_create(struct allocator* alloc)
{
        tree list = tree_create(alloc, tnk_list);
        if (!list)
                return NULL;
        tree_list_initf(list);
        return list;
}

extern union tree_node tree_list_initf(tree list)
{
        tree_kind(list) = tnk_list;
        list_initf(&__tree_list(list));
        return *list;
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

extern tree tree_sign_type_create(struct allocator* alloc, tree res, tree args)
{
        tree sign = tree_create(alloc, tnk_sign_type);
        if (!sign)
                return NULL;
        tree_sign_args(sign) = args;
        tree_sign_restype(sign) = res;
        return sign;
}
