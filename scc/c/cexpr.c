#include "cexpr.h"
#include "ccommon.h"
#include "ctype.h"
#include "ctree.h"

static inline int expect_exp(tree node)
{
        return node && tree_kind(node) == tnk_exp;
}

extern int c_node_is_operand(tree node)
{
        return expect_exp(node) 
                && (tree_exp_kind(node) == ok_null || tree_exp_kind(node) == ok_operand);
}
extern int c_node_is_prefix_operator(tree node)
{
        if (!expect_exp(node))
                return 0;
        switch (tree_exp_kind(node))
        {
                case ok_pre_inc:
                case ok_pre_dec:
                case ok_plus:
                case ok_minus:
                case ok_bit_not:
                case ok_not:
                case ok_dereference:
                case ok_address:
                        return 1;

                default:
                        return 0;
        }
}

extern int c_node_is_postfix_operator(tree node)
{
        if (!expect_exp(node))
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

extern int c_parser_token_is_const(struct c_parser* parser)
{
        switch (c_parser_token_type(parser))
        {
                case ctt_const_char:
                case ctt_const_float:
                case ctt_const_int:
                case ctt_const_string:
                        return 1;

                default:
                        return 0;
        }
}

extern int c_node_is_postfix_unary_operator(tree node)
{
        return expect_exp(node)
                && (tree_exp_kind(node) == ok_post_dec || tree_exp_kind(node) == ok_post_inc);
}

static inline enum operator_kind unary_operator_kind(enum c_token_type token, int prev_operand)
{
        if (token == ctt_inc)
                return prev_operand ? ok_post_inc : ok_pre_inc;
        if (token == ctt_dec)
                return prev_operand ? ok_post_dec : ok_pre_dec;
        if (prev_operand)
                return ok_null;

        switch (token)
        {
                case ctt_plus:    return ok_plus;
                case ctt_minus:   return ok_minus;
                case ctt_not:     return ok_not;
                case ctt_bit_not: return ok_bit_not;
                case ctt_asterix: return ok_dereference;
                case ctt_bit_and: return ok_address;

                default:
                        return ok_null;
        }
}

static inline enum operator_kind binary_operator_kind(enum c_token_type token, int prev_operand)
{
        if (token == ctt_lbracket && prev_operand)
                return ok_call;

        // 100% binary operator
        switch (token)
        {
                case ctt_lsbracket:  return ok_subscript;
                case ctt_div:        return ok_div;
                case ctt_mod:        return ok_mod;
                case ctt_dot:        return ok_member;
                case ctt_ptr:        return ok_member_ptr;
                case ctt_shl:        return ok_shl;
                case ctt_shr:        return ok_shr;
                case ctt_le:         return ok_le;
                case ctt_gr:         return ok_gr;
                case ctt_leq:        return ok_leq;
                case ctt_geq:        return ok_geq;
                case ctt_eq:         return ok_eq;
                case ctt_neq:        return ok_neq;
                case ctt_bit_xor:    return ok_bit_xor;
                case ctt_bit_or:     return ok_bit_or;
                case ctt_and:        return ok_and;
                case ctt_or:         return ok_or;
                case ctt_assign:     return ok_assign;
                case ctt_add_assign: return ok_add_assign;
                case ctt_sub_assign: return ok_sub_assign;
                case ctt_mul_assign: return ok_mul_assign;
                case ctt_div_assign: return ok_div_assign;
                case ctt_mod_assign: return ok_mod_assign;
                case ctt_shl_assign: return ok_shl_assign;
                case ctt_shr_assign: return ok_shr_assign;
                case ctt_and_assign: return ok_and_assing;
                case ctt_xor_assign: return ok_xor_assing;
                case ctt_or_assign:  return ok_or_assing;
                case ctt_coma:       return ok_coma;
        }

        if (!prev_operand)
                return token == ctt_lbracket ? ok_cast : ok_null;

        switch (token)
        {
                case ctt_asterix: return ok_mul;
                case ctt_plus:    return ok_add;
                case ctt_minus:   return ok_sub;
                case ctt_bit_and: return ok_bit_and;

                default:
                        return ok_null;
        }
}

static inline void set_right_to_null(tree exp, unsigned nesting)
{
        if (!tree_exp_right(exp))
                tree_exp_right(exp) = c_tree_exp_create(ok_null, nesting);
}

static inline void set_left_to_null(tree exp, unsigned nesting)
{
        if (!tree_exp_left(exp))
                tree_exp_left(exp) = c_tree_exp_create(ok_null, nesting);
}

static inline int c_node_is(tree it, enum operator_kind kind)
{
        if (!tree_list_iterator_valid(it))
                return 0;
        tree exp = tree_list_node_base(it);
        return tree_exp_kind(exp) == kind;
}

static inline int is_prev_operand(tree prev_it, unsigned nesting)
{
        if (!tree_list_iterator_valid(prev_it))
                return 0;
        tree exp = tree_list_node_base(prev_it);

        // type(*)... case
        if (tree_exp_kind(exp) == ok_dereference && tree_exp_nesting(exp) > nesting)
        {
                set_right_to_null(exp, nesting);
                return 1;
        }

        return tree_exp_kind(exp) == ok_operand || c_node_is_postfix_operator(exp);
}

static int parse_operand(struct c_parser* parser, struct c_symtab* symtab, tree list)
{
        tree op = NULL;
        unsigned nesting = c_parser_nesting(parser);
        int type = 0;

        if (c_parser_token_is_const(parser))
                op = c_tree_create_const(c_parser_token(parser));
        else if (c_parser_token_is(parser, ctt_identifier))
        {
                op = c_symtab_get_symb(symtab, hash64_str(c_parser_token_str(parser)));
                if (!op)
                        op = tree_ident_create(c_parser_token_str(parser));
        }
        else
        {
                op = c_parse_base_type(parser, symtab);
                if (!op)
                        return 0;
                type = 1;
        }

        tree node = c_tree_exp_create(ok_operand, nesting);
        tree_exp_next(node) = op;
        if (type)
        {
                tree etype = c_tree_exp_create(ok_type, nesting);
                tree_exp_left(etype) = node;
                node = etype;
        }
        else
                c_parser_advance(parser);
        tree_list_push_back(list, tree_list_node_create(node));
        return 1;
}

static inline int parse_unary_operator(struct c_parser* parser, struct c_symtab* symtab, int prev_operand, tree list)
{
        enum operator_kind kind = unary_operator_kind(c_parser_token_type(parser), prev_operand);
        if (!kind)
                return 0;
        unsigned nesting = c_parser_nesting(parser);
        tree unary = c_tree_exp_create(kind, nesting);

        if (c_node_is_prefix_operator(unary))
                set_left_to_null(unary, nesting);
        else if (c_node_is_postfix_unary_operator(unary))
                set_right_to_null(unary, nesting);

        tree_list_push_back(list, tree_list_node_create(unary));
        return 1;
}

static inline void set_right_if_prev_is_type_or_dereference(tree list, unsigned nesting)
{
        tree tail = tree_list_tail(list);
        if (c_node_is(tail, ok_type) || c_node_is(tail, ok_dereference))
                set_right_to_null(tree_list_node_base(tail), nesting);
}

static tree parse_expr_raw(struct c_parser* parser, struct c_symtab* symtab, size_t size);

static inline int parse_binary_operator(struct c_parser* parser, struct c_symtab* symtab, int prev_operand, tree list)
{
        enum operator_kind kind = binary_operator_kind(c_parser_token_type(parser), prev_operand);
        if (!kind)
                return 0;
        unsigned nesting = c_parser_nesting(parser);
        tree binary = c_tree_exp_create(kind, nesting);

        if (kind == ok_call || kind == ok_cast || kind == ok_subscript)
        {
                c_parser_save_state(parser);
                c_parser_advance(parser);
                size_t size = c_parser_distance_till_closing_bracket(parser);

                tree subexpr = kind == ok_cast
                        ? c_parse_base_type(parser, symtab)
                        : parse_expr_raw(parser, symtab, size);

                if (!subexpr && kind == ok_cast)
                {
                        c_parser_load_state(parser);
                        return 0;
                }

                c_parser_pop_state(parser);
                if (!subexpr)
                        subexpr = c_tree_exp_create(ok_null, nesting);
                if (kind == ok_call || kind == ok_subscript)
                        tree_exp_right(binary) = subexpr;
                else
                        tree_exp_left(binary) = subexpr;
        }
        else if (kind == ok_coma)
                set_right_if_prev_is_type_or_dereference(list, nesting);

        tree_list_push_back(list, tree_list_node_create(binary));
        return 1;
}

static int parse_operator(struct c_parser* parser, struct c_symtab* symtab, tree list)
{
        int prev_operand = is_prev_operand(tree_list_tail(list), c_parser_nesting(parser));
        if (!parse_unary_operator(parser, symtab, prev_operand, list))
                if (!parse_binary_operator(parser, symtab, prev_operand, list))
                        return 0;
        c_parser_advance(parser);
        return 1;
}

static void preprocess_expr(struct c_parser* parser, struct c_symtab* symtab, size_t size, tree res)
{
        size_t pos = c_parser_counter(parser);
        while (c_parser_counter(parser) < pos + size)
                if (!parse_operand(parser, symtab, res))
                        if (!parse_operator(parser, symtab, res))
                                c_parser_advance(parser);
        set_right_if_prev_is_type_or_dereference(res, 0);
}

static tree pop_expr(tree output)
{
        if (tree_list_empty(output))
                return NULL;
        tree node = tree_list_pop_back(output);
        tree exp = tree_list_node_base(node);
        tree_delete(node);
        return exp;
}

static void build_expr(tree enode, tree output)
{
        tree exp = tree_list_node_base(enode);
        if (!c_node_is_operand(exp))
        {
                if (!tree_exp_right(exp))
                        tree_exp_right(exp) = pop_expr(output);
                if (!tree_exp_left(exp))
                        tree_exp_left(exp) = pop_expr(output);
        }
        tree_list_push_back(output, enode);
}

static tree parse_expr_raw(struct c_parser* parser, struct c_symtab* symtab, size_t size)
{
        if (!size)
                return NULL;

        struct tree_node list = tree_list_init(&list);
        preprocess_expr(parser, symtab, size, &list);
        if (tree_list_empty(&list))
                return NULL;

        struct tree_node output = tree_list_init(&output);
        struct tree_node opstack = tree_list_init(&opstack);

        tree it = tree_list_iterator_init(&list);
        while (tree_list_iterator_valid(it))
        {
                tree enode = it;
                tree exp = tree_list_node_base(it);
                tree_list_iterator_advance(it);

                if (c_node_is_operand(exp))    
                        build_expr(enode, &output);
                else
                {
                        while (!tree_list_empty(&opstack))
                        {
                                tree tail = tree_list_tail(&opstack);
                                unsigned ecp = tree_exp_computed_precedence(exp);
                                unsigned tcp = tree_exp_computed_precedence(tree_list_node_base(tail));
                                if (tree_exp_assoc(exp) == oak_left_to_right && ecp <= tcp
                                 || tree_exp_assoc(exp) == oak_right_to_left && ecp <  tcp)
                                {
                                        tree_list_pop_back(&opstack);
                                        build_expr(tail, &output);
                                }
                                else
                                        break;
                        }
                        tree_list_push_back(&opstack, enode);
                }
        }
        while (!tree_list_empty(&opstack))
                build_expr(tree_list_pop_back(&opstack), &output);
        tree node = tree_list_pop_back(&output);
        tree exp = tree_list_node_base(node);
        tree_delete(node);
        return exp;
}

extern tree c_parse_expr_raw(struct c_parser* parser, struct c_symtab* symtab, size_t size)
{
        c_parser_enable_nesting_tracking(parser);
        tree exp = parse_expr_raw(parser, symtab, size);
        c_parser_disable_nesting_tracking(parser);
        return exp;
}

extern tree c_parse_expr(struct c_parser* parser, struct c_symtab* symtab, size_t size)
{
	return NULL;
}
