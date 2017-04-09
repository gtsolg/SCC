#ifndef C_PARSER_H
#define C_PARSER_H

#include "cparser.h"
#include "../tree/ctree.h"
#include "tree/tree_pass.h"

extern tree c_parse_base_type(struct c_parser* parser);
extern tree c_parse_type(struct c_parser* parser, size_t size);

extern tree c_parse_expr_raw(struct c_parser* parser, size_t size);
extern tree c_parse_expr(struct c_parser* parser, size_t size);

extern tree c_parse_decl(struct c_parser* parser);
extern tree c_parse_stmt(struct c_parser* parser);

#endif // !C_PARSER_H
