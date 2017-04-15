#ifndef TREE_INFO_H
#define TREE_INFO_H

#include "tree_node.h"
#include "tree_access.h"

// holds information about tree expression node, default values are appropriate for C language
extern const int tree_exp_info[ok_size];
extern const int tree_exp_precedence_info[ok_size];

enum tree_exp_node_info
{
        tei_left_assoc  = 1 << 0,
        tei_right_assoc = 1 << 1,

        tei_binary      = 1 << 2,
        tei_unary       = 1 << 3,
        tei_postfix     = 1 << 4,
        tei_prefix      = 1 << 5,

        tei_operand     = 1 << 4,
        tei_assign      = 1 << 5,
};

static inline int tree_is_exp(tree node)
{
        return node && tree_is(node, tnk_exp);
}

static inline int tree_exp_get_precedence(tree node)
{
        scc_assert(tree_is_exp(node));
        return tree_exp_precedence_info[tree_exp_kind(node)];
}

static inline int get_exp_info(tree node)
{
        scc_assert(tree_is_exp(node));
        return tree_exp_info[tree_exp_kind(node)];
}

static inline int tree_exp_is_assign(tree node)
{
        return get_exp_info(node) & tei_assign;
}

static inline int tree_exp_is_operand(tree node)
{
        return get_exp_info(node) & tei_operand;
}

static inline int tree_exp_is_left_assoc(tree node)
{
        return get_exp_info(node) & tei_left_assoc;
}

static inline int tree_exp_is_right_assoc(tree node)
{
        return get_exp_info(node) & tei_right_assoc;
}

static inline int tree_exp_is_unary(tree node)
{
        return get_exp_info(node) & tei_unary;
}

static inline int tree_exp_is_binary(tree node)
{
        return get_exp_info(node) & tei_binary;
}

static inline int tree_exp_is_postfix(tree node)
{
        return get_exp_info(node) & tei_postfix;
}

static inline int tree_exp_is_prefix(tree node)
{
        return get_exp_info(node) & tei_prefix;
}

static inline int tree_list_node_is_exp(tree node)
{
        scc_assert(node && tree_is(node, tnk_list_node));
        tree exp = tree_list_node_base(node);
        return tree_is_exp(exp);
}

#endif // !TREE_INFO_H
