#include "tree.h"
#include "scc.h"

extern tree tree_create(enum tree_node_kind kind)
{
        tree node = calloc(1, sizeof(*node));
        tree_kind(node) = kind;
        return node;
}

extern void tree_delete(tree node)
{
        free(node);
}

extern tree tree_type_create(enum type_kind kind, enum type_qualifier qual, tree type)
{
        tree t = tree_create(tnk_type);
        tree_type_kind(t) = kind;
        tree_type_qual(t) = qual;
        tree_type(t) = type;
        return t;
}

extern tree tree_ident_create(const char* name)
{
        tree id = tree_create(tnk_id);
        tree_id_name(id) = name;
        return id;
}

extern tree tree_list_node_create(tree base)
{
        tree node = tree_create(tnk_list_node);
        tree_list_node(node) = list_null_node;
        tree_list_node_base(node) = base;
        return node;
}

extern tree tree_exp_create(enum operator_kind kind, unsigned nesting, struct tree_exp_info* opinfo)
{
        tree exp = tree_create(tnk_exp);
        tree_exp_left(exp) = NULL;
        tree_exp_right(exp) = NULL;
        tree_exp_info(exp) = opinfo[kind];
        tree_exp_nesting(exp) = nesting;
        return exp;
}

extern tree tree_const_int_create(uint64_t val)
{
        tree cst = tree_create(tnk_const_int);
        tree_const_int(cst) = val;
        return cst;
}

extern tree tree_const_real_create(long double val)
{
        tree cst = tree_create(tnk_const_real);
        tree_const_real(cst) = val;
        return cst;
}

extern tree tree_const_float_create(float val)
{
        tree cst = tree_create(tnk_const_float);
        tree_const_float(cst) = val;
        return cst;
}

extern tree tree_const_string_create(const char* string)
{
        tree cst = tree_create(tnk_const_string);
        tree_const_string(cst) = string;
        return cst;
}
