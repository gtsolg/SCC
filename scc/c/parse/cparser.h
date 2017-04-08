#ifndef C_PARSER_INTERNAL
#define C_PARSER_INTERNAL

#include "../ccommon.h"
#include "../reader/creader.h"
#include "pool.h"
#include "tree/tree_index.h"

#define C_PARSER_MAX_STATES 256
#define C_PARSER_GLOBL_SCOPE_IDX (-1)

struct c_parser_state
{
        c_token* token;
        size_t nesting; // depth of parentheses nestings
        size_t counter; // token counter
};

struct c_parser
{
        struct c_parser_state state[C_PARSER_MAX_STATES];
        size_t state_idx;

        struct list_node token_list;

        struct c_reader* reader;

        struct tree_index* globl;
        struct tree_index scope[C_MAX_BLOCK_NESTING];
        size_t scope_idx;

        scc_err_t err;
        jmp_buf on_err_buf;

        struct pool* tree_pool;
        struct pool c_token_pool;
        struct allocator tree_alloc;
        struct allocator c_token_alloc;
};

#define c_parser_tree_alloc(parser)      ((parser)->tree_alloc)
#define c_parser_c_token_alloc(parser)   ((parser)->c_token_alloc)
#define c_parser_reader(parser)          ((parser)->reader)
#define c_parser_cur_state(parser)       ((parser)->state + (parser)->state_idx)
#define c_parser_cur_local_scope(parser) ((parser)->scope + (parser)->scope_idx)
#define c_parser_err(parser)             ((parser)->err)
#define c_parser_reader_eof(parser)      ((parser)->reader_eof)
#define c_parser_token(parser)           (c_parser_cur_state(parser).token)
#define c_parser_counter(parser)         (c_parser_cur_state(parser).counter)
#define c_parser_nesting(parser)         (c_parser_cur_state(parser).nesting)
#define c_parser_next_token(parser)      ((struct c_token*)c_parser_token(parser)->node.next)
#define c_parser_prev_token(parser)      ((struct c_token*)c_parser_token(parser)->node.prev)
#define c_parser_token_ref(parser)       (c_parser_token(parser)->ref)
#define c_parser_token_type(parser)      (c_parser_token(parser)->type)
#define c_parser_eof(parser)             (c_parser_token_type(parser) == ctt_eof)

#define c_parser_cur_scope(parser) \
        ((parser)->scope_idx == C_PARSER_GLOBL_SCOPE_IDX ? (parser)->globl : c_parser_cur_local_scope(parser))

#define c_parser_error_fence(parser) \
        if (c_parser_err(parser))    \
                longjmp((parser)->on_err_buf, c_parser_err(parser))

#define c_parser_handle_err(parser, err)      \
        do {                                  \
                c_parser_err(parser) = err;   \
                c_parser_error_fence(parser); \
        } while(0)

#define c_parser_set_on_err(parser) \
        if (c_parser_err(parser) || setjmp((parser)->on_err_buf))

extern void c_parser_init(struct c_parser* parser, struct tree_index* globl, struct c_reader* reader, struct pool* tree_pool);
extern void c_parser_init_first_token(struct c_parser* parser);
extern void c_parser_shutdown(struct c_parser* parser);
extern void c_parser_save_state(struct c_parser* parser);
extern void c_parser_pop_state(struct c_parser* parser);
extern void c_parser_load_state(struct c_parser* parser);

extern void c_parser_lex_token(struct c_parser* parser);
extern int  c_parser_advance(struct c_parser* parser);
extern int  c_parser_rewind(struct c_parser* parser);
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

extern void c_parser_enter_scope(struct c_parser* parser);
extern void c_parser_exit_scope(struct c_parser* parser);
extern void c_parser_add_decl(struct c_parser* parser, tree decl, strref_t name);
extern int  c_parser_decl_exists(struct c_parser* parser, strref_t name);
extern tree c_parser_get_decl(struct c_parser* parser, strref_t name);

#endif // !C_PARSER_INTERNAL
