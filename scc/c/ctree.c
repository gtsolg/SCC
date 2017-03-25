#include "ctree.h"

extern enum type_qualifier ctt_to_qualifier(enum c_token_type type)
{
        switch (type)
        {
                case ctt_const:    return tq_const;
                case ctt_volatile: return tq_volatile;

                default: return tq_unqualified;
        }
}

extern enum func_call_type ctt_to_call_type(enum c_token_type type)
{

}

extern enum decl_storage_class ctt_to_storage_class(enum c_token_type type)
{
        switch (type)
        {
                case ctt_auto:     return dsc_auto;
                case ctt_register: return dsc_register;
                case ctt_extern:   return dsc_register;
                case ctt_static:   return dsc_static;

                default: return dsc_auto;
        }
}

extern enum type_qualifier c_token_to_qualifier(const c_token* token)
{
        return ctt_to_qualifier(c_token_type(token));
}

extern enum decl_storage_class c_token_to_storage_class(const c_token* token)
{
        return ctt_to_storage_class(c_token_type(token));
}

extern int c_node_is_exp(tree node)
{
        return node && tree_kind(node) == tnk_exp;
}

extern int c_node_is_operand(tree node)
{
        if (!c_node_is_exp(node))
                return tree_kind(node) == tnk_null;
        switch (tree_exp_kind(node))
        {
                case ok_operand:
                case ok_attrib:
                        return 1;

                default:
                        return 0;
        }
}

extern int c_node_is_postfix_operator(tree node)
{
        if (!c_node_is_exp(node))
                return 0;
        switch (tree_exp_kind(node))
        {
                case ok_call:
                case ok_subscript:
                case ok_post_dec:
                case ok_post_inc:
                        return 1;

                default:
                        return 0;
        }
}

extern int c_list_node_is_exp(tree node, enum expr_node_kind op)
{
        if (tree_kind(node) != tnk_list_node)
                return 0;
        tree exp = tree_list_node_base(node);
        return c_node_is_exp(exp) && tree_exp_kind(exp) == op;
}

extern tree c_token_to_cst_tree(struct allocator* alloc, const c_token* token)
{
        switch (c_token_type(token))
        {
                case ctt_const_int:    return tree_const_int_create(alloc, token->integer);
                case ctt_const_float:  return tree_const_float_create(alloc, token->flt);
                case ctt_const_string: return tree_const_string_create(alloc, token->ref);

                default: return NULL;
        }
}

extern tree c_tree_exp_create(struct allocator* alloc, enum expr_node_kind op, unsigned nesting)
{
        return tree_exp_create(alloc, op, nesting, c_opinfo);
}
