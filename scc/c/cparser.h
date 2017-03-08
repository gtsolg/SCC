#ifndef C_PARSER_H
#define C_PARSER_H

#include "cparser_internal.h"

extern tree c_parse_base_type(struct c_parser* parser, struct c_symtab* symtab);
extern tree c_parse_type(struct c_parser* parser, struct c_symtab* symtab, size_t size);

extern int c_node_is_operand(tree node);
extern tree c_parse_expr_raw(struct c_parser* parser, struct c_symtab* symtab, size_t size);
extern tree c_parse_expr(struct c_parser* parser, struct c_symtab* symtab, size_t size);

extern tree c_parse_decl(struct c_parser* parser, struct c_symtab* symtab);
extern tree c_parse_stmt(struct c_parser* parser, struct c_symtab* symtab);

#endif // !C_PARSER_H
