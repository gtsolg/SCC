#include "ctree_build.h"

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

static tree build_type(struct allocator*, tree);

static inline tree build_type_list(struct allocator* alloc, tree exp)
{
        if (tree_is(exp, tnk_null))
                return TREE_NULL;

        tree list = tree_list_create(alloc);
        while (tree_exp_is(exp, ok_coma))
        {
                tree type = c_build_type(alloc, tree_exp_left(exp));
                if (!type)
                        goto cleanup;

                tree_list_push_back(list, tree_list_node_create(alloc, type));
                exp = tree_exp_right(exp);
        }
        tree last = c_build_type(alloc, exp);
        if (!last)
                goto cleanup;

        tree_list_push_back(list, tree_list_node_create(alloc, last));
        return list;

cleanup:
        // cleanup
        return NULL;
}

static inline tree build_function_type(struct allocator* alloc, tree it)
{
        tree args = build_type_list(alloc, tree_exp_right(tree_iterator_pos(it)));
        if (!args)
                return NULL;
        tree restype = build_type(alloc, c_type_iterator_advance(it));
        if (!restype)
                return NULL;

        tree sign = tree_sign_type_create(alloc, restype, args);
        return tree_type_create(alloc, tk_sign, tq_unqualified, sign);
}

static inline tree build_vector_type(struct allocator* alloc, tree it)
{
        tree size = tree_exp_node(tree_exp_right(tree_iterator_pos(it)));
        if (!tree_is(size, tnk_const_int))
                return NULL;

        tree type = build_type(alloc, c_type_iterator_advance(it));
        if (!type)
                return NULL;
        tree vec = tree_vector_type_create(alloc, type, tree_const_int(size));
        return tree_type_create(alloc, tk_vector, tq_unqualified, vec);
}

static inline tree build_pointer(struct allocator* alloc, tree it)
{
        enum type_qualifier qual = type_info_get_qual_and_reset(c_type_iterator_info(it));
        tree next = build_type(alloc, c_type_iterator_advance(it));
        if (!next)
                return NULL;
        return tree_type_create(alloc, tk_pointer, qual, next);
}

static inline tree build_base_type(struct allocator* alloc, tree it)
{
        struct c_type_build_info* info = c_type_iterator_info(it);
        tree base = tree_exp_node(tree_iterator_pos(it));

        enum type_qualifier qual = type_info_get_qual_and_reset(c_type_iterator_info(it));
        c_type_iterator_advance(it);
        qual |= type_info_get_qual_and_reset(c_type_iterator_info(it));

        if (tree_is(base, tnk_type) && !info->past_base)
        {
                info->past_base = 1;
                tree_type_qual(base) = qual;
                return base;
        }
        return NULL;
}

static tree build_type(struct allocator* alloc, tree it)
{
        if (!tree_iterator_node_kind(it))
                return TREE_NULL;
        switch (tree_exp_kind(tree_iterator_pos(it)))
        {
                case ok_dereference: return build_pointer(alloc, it);
                case ok_call:        return build_function_type(alloc, it);
                case ok_subscript:   return build_vector_type(alloc, it);
                case ok_operand:     return build_base_type(alloc, it);
                default:             return NULL;
        }
}

extern tree c_build_type(struct allocator* tree_alloc, tree raw)
{
        struct c_type_build_info info = c_type_build_info_init();
        struct tree_iterator it;
        tree res = NULL;
        if (c_type_iterator_initf(&it, raw, &info) == SCC_SUCCESS)
                res = build_type(tree_alloc, &it);
        return res;
}