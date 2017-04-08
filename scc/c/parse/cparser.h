#ifndef C_PARSER_H
#define C_PARSER_H

#include "cparser_internal.h"
#include "../ctree.h"

struct c_type_build_info
{
        enum decl_storage_class storage_class;
        enum func_call_type call_type;
        enum type_qualifier qual;

        unsigned past_base : 1;
        unsigned is_right_subtree : 1;
};

#define c_type_build_info_init() { dsc_auto, fct_auto, tq_unqualified, 0 }
#define c_type_iterator_info(ptree) ((struct c_type_build_info*)tree_iterator_data(ptree))

extern scc_err_t c_type_iterator_initf(tree it, tree exp, struct c_type_build_info* info);
extern tree      c_type_iterator_advance(tree it);
extern tree      c_parse_base_type(struct c_parser* parser, struct c_symtab* symtab);
extern tree      c_parse_type(struct c_parser* parser, struct c_symtab* symtab, size_t size);

extern tree c_parse_expr_raw(struct c_parser* parser, struct c_symtab* symtab, size_t size);
extern tree c_parse_expr(struct c_parser* parser, struct c_symtab* symtab, size_t size);

extern tree c_parse_decl(struct c_parser* parser, struct c_symtab* symtab);
extern tree c_parse_stmt(struct c_parser* parser, struct c_symtab* symtab);

#endif // !C_PARSER_H
