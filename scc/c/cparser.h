#ifndef C_PARSER_H
#define C_PARSER_H

#include "c/csymtab.h"
#include "c/clexer.h"

#define C_PARSER_MAX_STATES 256

struct c_parser_state
{
        struct c_token* token;
        size_t nesting; // depth of parentheses nestings
        size_t counter; // token counter
};

struct c_parser
{
        struct c_parser_state current;
        struct c_parser_state states[C_PARSER_MAX_STATES];
        size_t           state_idx;
        struct list_node token_list;
        struct c_lexer   lexer;
        unsigned         enable_nesting_tracking;
};

#define c_parser_cur_state(parser)  (parser)->current
#define c_parser_lexer(parser)      (parser)->lexer
#define c_parser_token(parser)      c_parser_cur_state(parser).token
#define c_parser_counter(parser)    c_parser_cur_state(parser).counter
#define c_parser_nesting(parser)    c_parser_cur_state(parser).nesting
#define c_parser_eof(parser)        ((parser)->lexer.eof && c_parser_token(parser) == list_end(&(parser)->token_list))
#define c_parser_next_token(parser) ((struct c_token*)c_parser_token(parser)->node.next)
#define c_parser_prev_token(parser) ((struct c_token*)c_parser_token(parser)->node.prev)
#define c_parser_token_str(parser)  c_parser_token(parser)->str
#define c_parser_token_type(parser) c_parser_token(parser)->type

extern void c_parser_init(struct c_parser* parser, const char* str);
extern void c_parser_lex_token(struct c_parser* parser);
extern void c_parser_save_state(struct c_parser* parser);
extern void c_parser_pop_state(struct c_parser* parser);
extern void c_parser_load_state(struct c_parser* parser);
extern void c_parser_enable_nesting_tracking(struct c_parser* parser);
extern void c_parser_disable_nesting_tracking(struct c_parser* parser);

extern int c_parser_advance(struct c_parser* parser);
extern int c_parser_rewind(struct c_parser* parser);
extern void c_parser_skip_tokens(struct c_parser* parser, size_t count);
extern void c_parser_rewind_tokens(struct c_parser* parser, size_t count);
extern size_t c_parser_tokens_remains(struct c_parser* parser);
extern size_t c_parser_skip_till(struct c_parser* parser, enum c_token_type type);

// assuming that parser is inside brackets
extern size_t c_parser_skip_till_closing_bracket(struct c_parser* parser);
extern size_t c_parser_distance_till_closing_bracket(struct c_parser* parser);

extern size_t c_parser_distance(struct c_parser* parser, enum c_token_type type);
extern int c_parser_next_token_is(struct c_parser* parser, enum c_token_type type);
extern int c_parser_prev_token_is(struct c_parser* parser, enum c_token_type type);
extern int c_parser_token_is(struct c_parser* parser, enum c_token_type type);
extern int c_parser_advance_if_next_token_is(struct c_parser* parser, enum c_token_type type);
extern int c_parser_advance_if_token_is(struct c_parser* parser, enum c_token_type type);
extern int c_parser_has_tokens(struct c_parser* parser, size_t count);

extern struct c_symtab* c_parse(struct c_parser* parser);

#endif // !C_PARSER_H
