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

extern tree c_tree_exp_create(enum operator_kind op, unsigned nesting)
{
        return tree_exp_create(op, nesting, c_opinfo);
}