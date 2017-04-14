#include "ctree_build.h"
#include "tree/tree_pass.h"

static inline void type_iterator_advance(tree it)
{
        if (c_type_iterator_info(it)->is_right_subtree)
                tree_exp_iterator_go_prev(it);
        else
                tree_exp_iterator_go_right(it);
}

static inline void type_iterator_skip_attribs(tree it)
{
        while (tree_exp_iterator_is(it, ok_attrib))
        {
                struct c_type_build_info* info = c_type_iterator_info(it);
                uint64_t att = tree_attrib(tree_exp_node(tree_iterator_pos(it)));
                info->qual |= ctt_to_qualifier(att);
                info->storage_class |= ctt_to_storage_class(att);
                type_iterator_advance(it);
        }
}

extern scc_err_t c_type_iterator_initf(tree it, tree exp, struct c_type_build_info* info)
{
        tree_iterator_pos(it) = tree_exp_left(exp);
        c_type_iterator_info(it) = info;
        info->is_right_subtree = 1;

        tree pos = tree_exp_right(exp);
        if (!tree_kind(pos))
                info->is_right_subtree = 0;
        else
                while (tree_kind(pos))
                {
                        enum exp_node_kind kind = tree_exp_kind(pos);
                        tree_iterator_pos(it) = pos;
                        if (kind == ok_call || kind == ok_subscript)
                                pos = tree_exp_left(pos);
                        else if (kind == ok_dereference || kind == ok_operand || kind == ok_attrib)
                                pos = tree_exp_next(pos);
                        else
                                return SCC_ERR;
                }
        type_iterator_skip_attribs(it);
        return SCC_SUCCESS;
}

extern tree c_type_iterator_advance(tree it)
{
        type_iterator_skip_attribs(it);
        type_iterator_advance(it);
        if (tree_exp_iterator_valid(it) && tree_exp_iterator_is(it, ok_type))
        {
                tree_exp_iterator_go_left(it);
                c_type_iterator_info(it)->is_right_subtree = 0;
        }
        type_iterator_skip_attribs(it);
        return it;
}

static inline enum type_qualifier type_info_get_qual_and_reset(struct c_type_build_info* info)
{
        enum type_qualifier qual = info->qual;
        info->qual = tq_unqualified;
        return qual;
}

static tree build_type(struct c_parser*, tree);

static inline tree build_type_list(struct c_parser* parser, tree exp)
{
        struct allocator* tree_alloc = &c_parser_tree_alloc(parser);

        if (tree_is(exp, tnk_null))
                return TREE_NULL;

        tree list = tree_list_create(tree_alloc);
        while (tree_exp_is(exp, ok_coma))
        {
                tree type = c_build_type(parser, tree_exp_left(exp));
                if (!type)
                        goto cleanup;

                tree_list_push_back_node(list, tree_alloc, type);
                exp = tree_exp_right(exp);
        }
        tree last = c_build_type(parser, exp);
        if (!last)
                goto cleanup;

        tree_list_push_back(list, tree_list_node_create(tree_alloc, last));
        return list;

cleanup:
        tree_delete_recursive(tree_alloc, list);
        return NULL;
}

static inline tree build_function_type(struct c_parser* parser, tree it)
{
        tree args = build_type_list(parser, tree_exp_right(tree_iterator_pos(it)));
        if (!args)
                return NULL;
        tree restype = build_type(parser, c_type_iterator_advance(it));
        if (!restype)
                return NULL;

        struct allocator* tree_alloc = &c_parser_tree_alloc(parser);
        tree sign = tree_sign_type_create(tree_alloc, restype, args);
        return tree_type_create(tree_alloc, tk_sign, tq_unqualified, sign);
}

static inline tree build_vector_type(struct c_parser* parser, tree it)
{
        tree size = tree_exp_node(tree_exp_right(tree_iterator_pos(it)));
        if (!tree_is(size, tnk_const_int))
                return NULL;

        tree type = build_type(parser, c_type_iterator_advance(it));
        if (!type)
                return NULL;

        struct allocator* tree_alloc = &c_parser_tree_alloc(parser);
        tree vec = tree_vector_type_create(tree_alloc, type, tree_const_int(size));
        return tree_type_create(tree_alloc, tk_vector, tq_unqualified, vec);
}

static inline tree build_pointer(struct c_parser* parser, tree it)
{
        enum type_qualifier qual = type_info_get_qual_and_reset(c_type_iterator_info(it));
        tree next = build_type(parser, c_type_iterator_advance(it));
        if (!next)
                return NULL;

        return tree_type_create(&c_parser_tree_alloc(parser), tk_pointer, qual, next);
}

static inline tree build_base_type(struct c_parser* parser, tree it)
{
        struct c_type_build_info* info = c_type_iterator_info(it);
        struct allocator* tree_alloc = &c_parser_tree_alloc(parser);

        if (info->past_base)
                return NULL;

        tree base = NULL;
        if (tree_iterator_node_kind(it) == tnk_id)
        {
                base = c_parser_get_decl(parser, tree_id_ref(tree_iterator_pos(it)));
                if (!base) // undefined identifier
                        return NULL;
                base = tree_type_create(tree_alloc, tk_declared, tq_unqualified, base);
        }
        else
                base = tree_copy(&c_parser_tree_alloc(parser), tree_exp_node(tree_iterator_pos(it)));

        enum type_qualifier qual = type_info_get_qual_and_reset(info);
        c_type_iterator_advance(it);
        qual |= type_info_get_qual_and_reset(info);

        info->past_base = 1;
        tree_type_qual(base) = qual;
        return base;
}

static tree build_type(struct c_parser* parser, tree it)
{
        tree pos = tree_iterator_pos(it);

        if (tree_is(pos, tnk_null))
                return TREE_NULL;

        if (tree_is(pos, tnk_id))
                return build_base_type(parser, it);

        scc_assert(tree_is(pos, tnk_exp));

        switch (tree_exp_kind(pos))
        {
                case ok_dereference: return build_pointer(parser, it);
                case ok_call:        return build_function_type(parser, it);
                case ok_subscript:   return build_vector_type(parser, it);
                case ok_operand:     return build_base_type(parser, it);

                default:
                        scc_assert(0);
                        return NULL;
        }
}

extern tree c_build_type(struct c_parser* parser, tree raw)
{
        struct c_type_build_info info = c_type_build_info_init();
        struct tree_iterator it;
        tree res = NULL;
        if (c_type_iterator_initf(&it, raw, &info) == SCC_SUCCESS)
                res = build_type(parser, &it);
        return res;
}

extern tree c_build_var_decl(struct c_parser* parser, tree raw)
{
        struct c_type_build_info info = c_type_build_info_init();
        struct tree_iterator it;
        struct allocator* tree_alloc = &c_parser_tree_alloc(parser);

        if (c_type_iterator_initf(&it, raw, &info) == SCC_ERR)
                return NULL;

        tree id = TREE_NULL;
        tree type = NULL;
        
        if (tree_iterator_node_kind(&it) == tnk_id)
        {
                id = tree_copy(tree_alloc, tree_iterator_pos(&it));
                c_type_iterator_advance(&it);
        }
        type = build_type(parser, &it);
        if (!type)
        {
                tree_delete(tree_alloc, id);
                return NULL;
        }

        return tree_var_decl_create(tree_alloc, id, type);
}

struct exp_stmt_build_info
{
        tree decls;
        tree base_type;
        scc_err_t status;
};

#define exp_stmt_build_info_init(decls, base_type) { decls, base_type, SCC_SUCCESS }

static int rebuild_raw_as_exp_stmt_pass(tree node, void* data)
{
        struct exp_stmt_build_info* info = data;

        if (info->status == SCC_ERR)
                return PASS_SHOULD_EXIT;
}

static int rebuild_raw_as_exp_stmt_match(tree node, void* data)
{
        if (!tree_is(node, tnk_exp))
                return 0;

        switch (tree_exp_kind(node))
        {
                case ok_assign:
                case ok_add_assign:
                case ok_sub_assign:
                case ok_mul_assign:
                case ok_div_assign:
                case ok_mod_assign:
                case ok_shl_assign:
                case ok_shr_assign:
                case ok_and_assign:
                case ok_xor_assign:
                case ok_or_assign:
                case ok_coma:
                        return 1;

                default:
                        return 0;
        }
}

extern tree c_rebuild_raw_as_exp_stmt(struct c_parser* parser, tree raw)
{
        struct allocator* tree_alloc = &c_parser_tree_alloc(parser);
        struct exp_stmt_build_info info = exp_stmt_build_info_init(tree_list_create(tree_alloc), TREE_NULL);

        tree_foreach(tree_alloc, raw,
                tree_pass_initf(rebuild_raw_as_exp_stmt_pass, &info, PASS_PREFS_NONE),
                tree_match_initf(rebuild_raw_as_exp_stmt_match, NULL));

        if (info.status == SCC_ERR)
        {
                tree_delete_list_nodes(tree_alloc, info.decls);
                tree_delete_recursive(tree_alloc, raw);
                return NULL;
        }

        return tree_exp_stmt_create(tree_alloc, info.decls, raw);
}