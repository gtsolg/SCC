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

static void delete_null_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_stmt_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_id_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_decl_node(struct allocator* alloc, tree decl)
{
        return;
}

static void delete_func_decl_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_exp_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_list_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_list_node_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_const_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_vector_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_sign_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_type_node(struct allocator* alloc, tree node)
{
        return;
}

static void delete_attrib_node(struct allocator* alloc, tree node)
{
        return;
}

static void(*tree_delete_fn[])(struct allocator*, tree) = 
{
        delete_null_node,      // tnk_null
        delete_stmt_node,      // tnk_stmt
        delete_id_node,        // tnk_id
        delete_decl_node,      // tnk_type_decl
        delete_decl_node,      // tnk_var_decl
        delete_func_decl_node, // tnk_func_decl
        delete_exp_node,       // tnk_exp
        delete_list_node,      // tnk_list
        delete_list_node_node, // tnk_list_node
        delete_const_node,     // tnk_const_int
        delete_const_node,     // tnk_const_float
        delete_const_node,     // tnk_const_real
        delete_const_node,     // tnk_const_string
        delete_vector_node,    // tnk_vector_type
        delete_sign_node,      // tnk_sign_type
        delete_type_node,      // tnk_type
        delete_attrib_node     // tnk_attrib,
};

extern void tree_delete(struct allocator* alloc, tree node)
{
        if (!node)
                return;

        enum tree_node_kind kind = tree_kind(node);
        scc_assert(kind >= tnk_null && kind <= tnk_attrib);
        tree_delete_fn[kind](alloc, node);
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

extern tree tree_list_create(struct allocator* alloc)
{
        tree list = tree_create(alloc, tnk_list);
        if (!list)
                return NULL;
        tree_list_initf(list);
        return list;
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