#include "cparser.h"

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
                struct c_type_parse_info* info = c_type_iterator_info(it);
                uint64_t att = tree_attrib(tree_exp_node(tree_iterator_pos(it)));
                info->qual |= ctt_to_qualifier(att);
                info->storage_class |= ctt_to_storage_class(att);
                type_iterator_advance(it);
        }
}


extern scc_err_t c_type_iterator_initf(tree it, tree exp, struct c_type_parse_info* info)
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

        //if (!tree_exp_iterator_valid(it))
        //        return it;

        //tree p = tree_iterator_pos(it);

        //if (!tree_exp_iterator_valid(it))
        //        return it;

        //if (tree_exp_iterator_is(it, ok_attrib))
        //        type_iterator_skip_attribs(it);
        //else
        //        type_iterator_advance(it);
        //p = tree_iterator_pos(it);
        //if (tree_exp_iterator_valid(it) && tree_exp_iterator_is(it, ok_type))
        //{
        //        tree_exp_iterator_go_left(it);
        //        c_type_iterator_info(it)->is_right_subtree = 0;
        //        type_iterator_skip_attribs(it);
        //}
        //return it;
}

static tree c_parse_default_type(struct c_parser* parser)
{
        uint64_t key = 0;
        while (!c_parser_eof(parser) && c_token_is_default_type(c_parser_token(parser)))
        {
                key += c_reswords_hash[c_parser_token_type(parser)];
                c_parser_advance(parser);
        }
        tree node = htab_get(&c_const_tree, key);
        if (!node)
                return NULL;// unknown default type
        return node;
}

static tree c_parse_alias(struct c_parser* parser, struct c_symtab* symtab)
{
        enum c_token_type composite = 0;
        if (c_token_is_composite_type(c_parser_token(parser)))
        {
                composite = c_parser_token_is(parser, ctt_struct) ? ctt_struct : ctt_union;
                c_parser_advance(parser);
        }
        if (!c_parser_token_is(parser, ctt_identifier))
                return NULL; // unknown token

        tree alias = c_symtab_get_symb(symtab, c_parser_token_ref(parser));
        if (!alias || tree_kind(alias) != tnk_type)
                return NULL; // unknown type or another symb instead of alias

        if (composite
         && (composite == ctt_struct && tree_type_kind(alias) != tk_record
         ||  composite == ctt_union  && tree_type_kind(alias) != tk_union))
        {
                // got struct/union + existing alias
                return NULL;
        }
        return alias;
}

extern tree c_parse_base_type(struct c_parser* parser, struct c_symtab* symtab)
{
        c_parser_save_state(parser);
        tree type = c_token_is_default_type(c_parser_token(parser))
                ? c_parse_default_type(parser)
                : c_parse_alias(parser, symtab);
        if (!type)
        {
                c_parser_load_state(parser);
                return NULL;
        }
        c_parser_pop_state(parser);
        return tree_copy(&c_parser_tree_alloc(parser), type);
}

#include "tree_print.h"

static tree build_type(struct allocator* alloc, tree it);

static inline enum type_qualifier type_info_get_qual_and_reset(struct c_type_parse_info* info)
{
        enum type_qualifier qual = info->qual;
        info->qual = tq_unqualified;
        return qual;
}

static inline tree build_type_list(struct allocator* alloc, tree exp)
{

}

static inline tree build_function_type(struct allocator* alloc, tree it)
{
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
        struct c_type_parse_info* info = c_type_iterator_info(it);
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

extern tree c_parse_type(struct c_parser* parser, struct c_symtab* symtab, size_t size)
{
        struct allocator* alloc = &c_parser_tree_alloc(parser);
        tree raw = c_parse_expr_raw(parser, symtab, size);
        if (!raw)
                return NULL;
        tree res = NULL;
        struct c_type_parse_info info = c_type_parse_info_init();

        struct tree_iterator it;
        if (c_type_iterator_initf(&it, raw, &info) == SCC_SUCCESS)
                res = build_type(&c_parser_tree_alloc(parser), &it);
        // cleanup
        return res;
}
