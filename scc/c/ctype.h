#ifndef C_TYPE_H
#define C_TYPE_H

#include "cparser.h"

extern int c_parser_token_is_default_type(struct c_parser* parser);
extern int c_parser_token_is_composite_type(struct c_parser* parser);
extern int c_parser_token_is_type_qualifier(struct c_parser* parser);

extern enum type_qualifier c_parse_type_qualifier(struct c_parser* parser);
extern tree c_parse_default_type(struct c_parser* parser);
extern tree c_parse_alias(struct c_parser* parser, struct c_symtab* symtab);
extern tree c_parse_base_type(struct c_parser* parser, struct c_symtab* symtab);
extern tree c_parse_type(struct c_parser* parser, struct c_symtab* symtab, size_t size);

#endif // !C_TYPE_H
