#include "cparse.h"

static inline enum expr_node_kind unary_operator_kind(enum c_token_type token, int prev_operand)
{
        if (token == ctt_inc)
                return prev_operand ? ok_post_inc : ok_pre_inc;
        if (token == ctt_dec)
                return prev_operand ? ok_post_dec : ok_pre_dec;
        if (prev_operand)
                return -1;

        switch (token)
        {
                case ctt_plus:    return ok_plus;
                case ctt_minus:   return ok_minus;
                case ctt_not:     return ok_not;
                case ctt_bit_not: return ok_bit_not;
                case ctt_asterix: return ok_dereference;
                case ctt_bit_and: return ok_address;

                default:
                        return -1;
        }
}

static inline enum expr_node_kind binary_operator_kind(enum c_token_type token, int prev_operand)
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
                case ctt_and_assign: return ok_and_assign;
                case ctt_xor_assign: return ok_xor_assign;
                case ctt_or_assign:  return ok_or_assign;
                case ctt_coma:       return ok_coma;
        }

        if (!prev_operand)
                return token == ctt_lbracket ? ok_cast : -1;

        switch (token)
        {
                case ctt_asterix: return ok_mul;
                case ctt_plus:    return ok_add;
                case ctt_minus:   return ok_sub;
                case ctt_bit_and: return ok_bit_and;

                default:
                        return -1;
        }
}

static inline void set_right_to_null(tree exp)
{
        if (!tree_exp_right(exp))
                tree_exp_right(exp) = TREE_NULL;
}

static inline void set_left_to_null(tree exp)
{
        if (!tree_exp_left(exp))
                tree_exp_left(exp) = TREE_NULL;
}

static inline int prev_list_node_is_operand(tree prev, unsigned nesting)
{
        if (tree_kind(prev) != tnk_list_node)
                return 0;
        tree exp = tree_list_node_base(prev);

        // type(*)... case
        if (tree_exp_is(exp, ok_dereference) && tree_exp_nesting(exp) > nesting)
        {
                set_right_to_null(exp);
                return 1;
        }
        return tree_exp_is(exp, ok_operand) || c_node_is_postfix_operator(exp);
}

static void parse_attribs_list(struct c_parser* parser, tree* head, tree* tail)
{
        *head = NULL;
        *tail = NULL;  
        struct allocator* alloc = &c_parser_tree_alloc(parser);

        while (c_token_is_exp_modifier(c_parser_token(parser)))
        {
                tree att = tree_attrib_create(alloc, c_parser_token_type(parser));
                tree exp = c_tree_exp_create(alloc, ok_attrib, c_parser_nesting(parser));
                tree_exp_node(exp) = att;
                if (!*head)
                {
                        *head = exp;
                        *tail = exp;
                }
                else
                {
                        tree_exp_prev(exp) = *tail;
                        tree_exp_next(*tail) = exp;
                        *tail = exp;
                }
                c_parser_advance(parser);
        }
}

static inline tree merge_attrib_lists(tree post_head, tree mid, tree pre_head, tree pre_tail)
{
        tree res = mid;
        if (post_head)
        {
                tree_exp_prev(post_head) = mid;
                tree_exp_next(mid) = post_head;
        }
        if (pre_head)
        {
                tree_exp_next(pre_tail) = mid;
                tree_exp_prev(mid) = pre_tail;
                res = pre_head;
        }
        return res;
}

static tree token_to_operand(struct c_parser* parser, unsigned nesting, int* is_type)
{
        const c_token* token = c_parser_token(parser);
        struct allocator* alloc = &c_parser_tree_alloc(parser);
        tree op = NULL;

        tree pre_att_head;
        tree pre_att_tail;
        parse_attribs_list(parser, &pre_att_head, &pre_att_tail);

        if (c_token_is_const_val(token))
                op = c_token_to_cst_tree(alloc, token);
        else if (c_parser_token_is(parser, ctt_identifier))
        {
                op = c_parser_get_decl(parser, c_token_ref(token));
                if (!op)
                        op = tree_ident_create(alloc, c_token_ref(token));
        }
        else
        {
                *is_type = 1;
                op = c_parse_base_type(parser);
        }
        
        if (!op)
        {
                tree_delete_recursive(alloc, pre_att_tail);
                return NULL;
        }

        tree post_att_head;
        tree post_att_tail;
        parse_attribs_list(parser, &post_att_head, &post_att_tail);

        tree node = c_tree_exp_create(alloc, ok_operand, nesting);
        tree_exp_node(node) = op;
        return merge_attrib_lists(post_att_head, node, pre_att_head, pre_att_tail);
}

static int parse_operand(struct c_parser* parser, tree list)
{
        struct allocator* alloc = &c_parser_tree_alloc(parser);
        unsigned nesting = c_parser_nesting(parser);
        int is_type = 0;

        tree node = token_to_operand(parser, nesting, &is_type);
        if (!node)
                return 0;

        if (is_type)
        {
                tree etype = c_tree_exp_create(alloc, ok_type, nesting);
                tree_exp_left(etype) = node;
                tree_exp_prev(node) = etype;
                node = etype;
        }
        else
                c_parser_advance(parser);
        tree_list_push_back(list, tree_list_node_create(alloc, node));
        return 1;
}

static inline int parse_unary_operator(struct c_parser* parser, int prev_operand, tree list)
{
        enum expr_node_kind kind = unary_operator_kind(c_parser_token_type(parser), prev_operand);
        if (kind == -1)
                return 0;

        unsigned nesting = c_parser_nesting(parser);
        tree unary = c_tree_exp_create(&c_parser_tree_alloc(parser), kind, nesting);
        set_left_to_null(unary);

        c_parser_advance(parser);
        tree post_att_head;
        tree post_att_tail;
        // int* att ... case
        parse_attribs_list(parser, &post_att_head, &post_att_tail);
        unary = merge_attrib_lists(post_att_head, unary, NULL, NULL);
 
        tree_list_push_back(list, tree_list_node_create(&c_parser_tree_alloc(parser), unary));
        return 1;
}

static inline void set_right_if_prev_is_type_or_dereference(tree list)
{
        tree tail = tree_list_tail(list);
        if (c_list_node_is_exp(tail, ok_type) || c_list_node_is_exp(tail, ok_dereference))
                set_right_to_null(tree_list_node_base(tail));
}

static inline int parse_binary_operator(struct c_parser* parser, int prev_operand, tree list)
{
        enum expr_node_kind kind = binary_operator_kind(c_parser_token_type(parser), prev_operand);
        if (kind == -1)
                return 0;
        unsigned nesting = c_parser_nesting(parser);
        tree binary = c_tree_exp_create(&c_parser_tree_alloc(parser), kind, nesting);

        if (kind == ok_call || kind == ok_cast || kind == ok_subscript)
        {
                c_parser_save_state(parser);
                c_parser_advance(parser);
                size_t size = c_parser_distance_till_closing_bracket(parser);

                tree subexpr = kind == ok_cast
                        ? c_parse_base_type(parser)
                        : c_parse_expr_raw(parser, size);

                if (!subexpr && kind == ok_cast)
                {
                        c_parser_load_state(parser);
                        return 0;
                }

                c_parser_pop_state(parser);
                if (!subexpr)
                        subexpr = TREE_NULL;
                if (kind == ok_call || kind == ok_subscript)
                        tree_exp_right(binary) = subexpr;
                else
                        tree_exp_left(binary) = subexpr;
        }
        else if (kind == ok_coma)
                set_right_if_prev_is_type_or_dereference(list);

        tree_list_push_back(list, tree_list_node_create(&c_parser_tree_alloc(parser), binary));
        c_parser_advance(parser);
        return 1;
}

static int parse_operator(struct c_parser* parser, tree list)
{
        int prev_operand = prev_list_node_is_operand(tree_list_tail(list), c_parser_nesting(parser));
        if (!parse_unary_operator(parser, prev_operand, list))
                if (!parse_binary_operator(parser, prev_operand, list))
                        return 0;
        return 1;
}

static void preprocess_expr(struct c_parser* parser, size_t size, tree res)
{
        size_t pos = c_parser_counter(parser);
        while (c_parser_counter(parser) < pos + size)
                if (!parse_operand(parser, res))
                        if (!parse_operator(parser, res))
                                c_parser_advance(parser);
        set_right_if_prev_is_type_or_dereference(res);
}

static inline tree* skip_attribs(tree* subexp, tree* parent)
{
        tree* it = subexp;
        while (c_node_is_exp(*it) && tree_exp_is(*it, ok_attrib))
                it = &tree_exp_next(*it);
        return it;
}

static inline void set_sibling(struct allocator* alloc, tree parent, int right, tree output)
{
        tree* member_ptr = right ? &tree_exp_right(parent) : &tree_exp_left(parent);
        ptrdiff_t member_off = member_ptr - parent;

        // since we can have attribs after dereference (e.g: int * const * ...)
        // we need to skip them in order to set the member
        tree* member = skip_attribs(member_ptr, &parent);
        parent = (tree)(member - member_off);

        if (*member)
                return;
        if (tree_list_empty(output))
                *member = TREE_NULL;
        else
        {
                tree back = tree_list_pop_back(output);
                tree exp = tree_list_node_base(back);
                tree_delete(alloc, back);
                tree_exp_prev(exp) = parent;
                *member = exp;
        }
}

static void build_expr(struct allocator* alloc, tree enode, tree output)
{
        tree exp = tree_list_node_base(enode);
        if (!c_node_is_operand(exp))
        {
                set_sibling(alloc, exp, 1, output);
                set_sibling(alloc, exp, 0, output);
        }
        tree_list_push_back(output, enode);
}

static void replace_null_pointers_pass(tree exp, void* p)
{
        if (!exp)
                return;
        if (!tree_exp_left(exp))
                tree_exp_left(exp) = TREE_NULL;
        if (!tree_exp_right(exp))
                tree_exp_right(exp) = TREE_NULL;
}

extern tree c_parse_expr_raw(struct c_parser* parser, size_t size)
{
        if (!size)
                return NULL;

        struct tree_node list = tree_list_initf(&list);
        preprocess_expr(parser, size, &list);
        if (tree_list_empty(&list))
                return NULL;

        struct tree_node output = tree_list_initf(&output);
        struct tree_node opstack = tree_list_initf(&opstack);
        struct allocator* tree_alloc = &c_parser_tree_alloc(parser);

        struct tree_iterator it = tree_list_iterator_init(&list);
        while (tree_list_iterator_valid(&it))
        {
                tree enode = tree_iterator_pos(&it);
                tree exp = tree_list_iterator_node(&it);
                tree_list_iterator_advance(&it);

                if (c_node_is_operand(exp))    
                        build_expr(tree_alloc, enode, &output);
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
                                        build_expr(tree_alloc, tail, &output);
                                }
                                else
                                        break;
                        }
                        tree_list_push_back(&opstack, enode);
                }
        }
        while (!tree_list_empty(&opstack))
                build_expr(tree_alloc, tree_list_pop_back(&opstack), &output);

        tree node = tree_list_pop_back(&output);
        tree exp = tree_list_node_base(node);
        tree_delete(tree_alloc, node);
        tree_foreach_alloc(tree_alloc, exp, replace_null_pointers_pass, NULL, TREE_PASS_NONE);
        return exp;
}

extern tree c_parse_expr(struct c_parser* parser, size_t size)
{
	return NULL;
}
