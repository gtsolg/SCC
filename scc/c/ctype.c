#include "ctype.h"
#include "ccommon.h"
#include "cexpr.h"

extern int c_parser_token_is_default_type(struct c_parser* parser)
{
        if (!c_parser_token(parser))
                return 0;
        switch (c_parser_token_type(parser))
        {
                case ctt_char:
                case ctt_short:
                case ctt_int:
                case ctt_long:
                case ctt_float:
                case ctt_double:
                case ctt_signed:
                case ctt_unsigned:
                case ctt_void:
                        return 1;

                default:
                        return 0;
        }
}

extern int c_parser_token_is_composite_type(struct c_parser* parser)
{
        enum c_token_type t = c_parser_token_type(parser);
        return t == ctt_struct || t == ctt_union;
}

extern int c_parser_token_is_type_qualifier(struct c_parser* parser)
{
        enum c_token_type t = c_parser_token_type(parser);
        return t == ctt_const || t == ctt_volatile;
}

extern enum type_qualifier c_parse_type_qualifier(struct c_parser* parser)
{
        enum type_qualifier qual = tq_unqualified;
        while (c_parser_token_is_type_qualifier(parser))
        {
                enum c_token_type token = c_parser_token_type(parser);
                if (token == ctt_const)
                        qual |= tq_const;
                else if (token == ctt_volatile)
                        qual |= tq_volatile;
                c_parser_advance(parser);
        }
        return qual;
}

extern tree c_parse_default_type(struct c_parser* parser)
{
        uint64_t key = 0;
        while (c_parser_token_is_default_type(parser))
        {
                key += c_reswords_hash[c_parser_token_type(parser)];
                c_parser_advance(parser);
        }
        tree node = htab_get(&c_const_tree, key);
        if (!node)
                return NULL;// unknown default type
        return node;
}

extern tree c_parse_alias(struct c_parser* parser, struct c_symtab* symtab)
{
        enum c_token_type composite = 0;
        if (c_parser_token_is_composite_type(parser))
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
        enum type_qualifier qual = c_parse_type_qualifier(parser);
        tree type = c_parser_token_is_default_type(parser)
                ? c_parse_default_type(parser)
                : c_parse_alias(parser, symtab);
        if (!type)
                goto backup;
        if (tree_type_qual(type) != qual)
                type = tree_type_create(&c_parser_tree_alloc(parser), tree_type_kind(type), qual, type);
        c_parser_pop_state(parser);
        return type;

backup:
        c_parser_load_state(parser);
        return NULL;
}

extern tree c_parse_type(struct c_parser* parser, struct c_symtab* symtab, size_t size)
{
        tree raw = c_parse_expr_raw(parser, symtab, size);
        if (!raw)
                return NULL;


        return NULL;
}
