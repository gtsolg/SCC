#include "cparse.h"
#include "../tree/ctree_build.h"

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

extern tree c_parse_type(struct c_parser* parser, struct c_symtab* symtab, size_t size)
{
        tree raw = c_parse_expr_raw(parser, symtab, size);
        if (!raw)
                return NULL;
        tree res = c_build_type(&c_parser_tree_alloc(parser), raw);
        // cleanup
        return res;
}
