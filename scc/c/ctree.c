#include "ctree.h"
#include "ccommon.h"

extern tree c_tree_create_const(const struct c_token* cst_token)
{
        switch (cst_token->type)
        {
                case ctt_const_char:
                case ctt_const_int:
                        return tree_const_int_create(cst_token->u64);

                case ctt_const_float:
                        return tree_const_float_create(cst_token->flt);

                case ctt_const_string:
                        return tree_const_string_create(cst_token->str);

                default:
                        return NULL;
        }
}

extern tree c_null_exp_node_create()
{
        return c_exp_list_node_create(ok_null, 0);
}

extern tree c_exp_list_node_create(enum operator_kind op, unsigned nesting)
{
        tree exp = tree_exp_create(op, nesting, c_opinfo);
        return tree_list_node_create(exp);
}

extern tree c_unexp_list_node_create(enum operator_kind op, unsigned nesting, tree next)
{
        tree exp = tree_exp_create(op, nesting, c_opinfo);
        tree_exp_next(exp) = next;
        return tree_list_node_create(exp);
}