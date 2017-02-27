#ifndef C_EXPR_H
#define C_EXPR_H

#include "cparser.h"

extern int c_node_is_operand(tree node);
extern int c_node_is_prefix_operator(tree node);
extern int c_node_is_postfix_operator(tree node);
extern int c_node_is_postfix_unary_operator(tree node);

extern int c_parser_token_is_const(struct c_parser* parser);

extern tree c_parse_expr_raw(struct c_parser* parser, struct c_symtab* symtab, size_t size);
extern tree c_parse_expr(struct c_parser* parser, struct c_symtab* symtab, size_t size);

#endif // !C_EXPR_H
