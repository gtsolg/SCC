#include "cparser.h"

static inline void* parser_get_obj(struct c_parser* parser, struct pool* pool)
{
        void* obj = pool_get(pool);
        if (!obj)
                c_parser_handle_err(parser, SCC_ERR);
        return obj;
}

static tree parser_alloc_tree(struct c_parser* parser, size_t size)
{
        return parser_get_obj(parser, parser->tree_pool);
}

static void parser_ret_tree(struct c_parser* parser, tree node)
{
        pool_return(parser->tree_pool, node);
}

static c_token* parser_alloc_ctoken(struct c_parser* parser, size_t size)
{
        return parser_get_obj(parser, &parser->c_token_pool);
}

static void parser_ret_ctoken(struct c_parser* parser, c_token* token)
{
        pool_return(&parser->c_token_pool, token);
}

static struct obj_info* parser_alloc_objinfo(struct c_parser* parser, size_t size)
{
        return parser_get_obj(parser, &parser->obj_pool);
}

static void parser_ret_objinfo(struct c_parser* parser, struct obj_info* info)
{
        pool_return(&parser->obj_pool, info);
}

extern void c_parser_init(struct c_parser* parser, struct c_reader* reader, struct pool* tree_pool)
{
        parser->state_idx = 0;
        parser->reader_eof = 0;
        c_parser_err(parser) = SCC_SUCCESS;
        c_parser_reader(parser) = reader;
        c_parser_counter(parser) = 0;
        c_parser_nesting(parser) = 0;
        c_parser_token(parser) = NULL;
        c_parser_disable_nesting_tracking(parser);

        list_initf(&parser->token_list);

        parser->tree_pool = tree_pool;
        pool_initf(&parser->c_token_pool, sizeof(c_token), 10, STD_ALLOC);

        alloc_initf(&parser->tree_alloc,    parser, parser_alloc_tree,   parser_ret_tree);
        alloc_initf(&parser->c_token_alloc, parser, parser_alloc_ctoken, parser_ret_ctoken);
        alloc_initf(&parser->obj_alloc,     parser, parser_get_obj,      parser_ret_objinfo);
}

extern void c_parser_shutdown(struct c_parser* parser)
{
        pool_delete(&parser->obj_pool);
        pool_delete(&parser->c_token_pool);
}

extern void c_parser_save_state(struct c_parser* parser)
{
        if (parser->state_idx >= C_PARSER_MAX_STATES)
                c_parser_handle_err(parser, SCC_ERR); // expression is too complex
        parser->states[parser->state_idx++] = c_parser_cur_state(parser);
}

extern void c_parser_pop_state(struct c_parser* parser)
{
        parser->state_idx--;
        assert(parser->state_idx != -1);
}

extern void c_parser_load_state(struct c_parser* parser)
{
        --parser->state_idx;
        assert(parser->state_idx != -1);
        c_parser_cur_state(parser) = parser->states[parser->state_idx];
}

extern void c_parser_enable_nesting_tracking(struct c_parser* parser)
{
        parser->enable_nesting_tracking = 1;
}

extern void c_parser_disable_nesting_tracking(struct c_parser* parser)
{
        parser->enable_nesting_tracking = 0;
}

extern int c_parser_advance(struct c_parser* parser)
{
        if (parser->enable_nesting_tracking)
        {
                enum c_token_type type = c_parser_token_type(parser);
                if (type == ctt_lbracket || type == ctt_lsbracket)
                        c_parser_nesting(parser)++;
                if (type == ctt_rbracket || type == ctt_rsbracket)
                        c_parser_nesting(parser)--;
        }
        c_parser_counter(parser)++;
        if (c_parser_next_token(parser) == list_end(&parser->token_list))
                c_parser_lex_token(parser);
        c_parser_token(parser) = c_parser_next_token(parser);
        return !c_parser_eof(parser);
}

extern int c_parser_rewind(struct c_parser* parser)
{
        if (parser->enable_nesting_tracking)
        {
                enum c_token_type type = c_parser_token_type(parser);
                if (type == ctt_lbracket || type == ctt_lsbracket)
                        c_parser_nesting(parser)--;
                if (type == ctt_rbracket || type == ctt_rsbracket)
                        c_parser_nesting(parser)++;
        }
        c_parser_counter(parser)--;
        c_parser_token(parser) = c_parser_prev_token(parser);
        return !c_parser_eof(parser);
}

extern void c_parser_skip_tokens(struct c_parser* parser, size_t count)
{
        while (count)
        {
                c_parser_advance(parser);
                count--;
        }
}

extern void c_parser_rewind_tokens(struct c_parser* parser, size_t count)
{
        while (count)
        {
                c_parser_rewind(parser);
                count--;
        }
}

extern size_t c_parser_tokens_remains(struct c_parser* parser)
{
        size_t count = 1;
        c_parser_save_state(parser);
        while (c_parser_advance(parser))
                count++;
        c_parser_load_state(parser);
        return count;
}

extern size_t c_parser_skip_till(struct c_parser* parser, enum c_token_type type)
{
        size_t counter = 0;
        while (!c_parser_token_is(parser, type))
        {
                if (c_parser_eof(parser))
                        return 0;
                c_parser_advance(parser);
                counter++;
        }
        return counter;
}


extern size_t c_parser_skip_till_closing_bracket(struct c_parser* parser)
{
        size_t nesting = c_parser_nesting(parser);
        size_t counter = 0;

        while (1)
        {
                if (c_parser_eof(parser))
                        return 0;
                enum c_token_type t = c_parser_token_type(parser);
                if (t == ctt_rbracket || t == ctt_rsbracket)
                        if (c_parser_nesting(parser) == nesting)
                                return counter;
                counter++;
                c_parser_advance(parser);
        }
}

extern size_t c_parser_distance_till_closing_bracket(struct c_parser* parser)
{
        c_parser_save_state(parser);
        size_t d = c_parser_skip_till_closing_bracket(parser);
        c_parser_load_state(parser);
        return d;
}

extern size_t c_parser_distance(struct c_parser* parser, enum c_token_type type)
{
        c_parser_save_state(parser);
        size_t d = c_parser_skip_till(parser, type);
        c_parser_load_state(parser);
        return d;
}

extern int c_parser_token_is(struct c_parser* parser, enum c_token_type type)
{
        return c_parser_token_type(parser) == type;
}

extern int c_parser_next_token_is(struct c_parser* parser, enum c_token_type type)
{
        if (!c_parser_has_tokens(parser, 1))
                return 0;
        struct token* token = c_parser_next_token(parser);
        return token->type == type;
}

extern int c_parser_prev_token_is(struct c_parser* parser, enum c_token_type type)
{
        struct token* token = c_parser_prev_token(parser);
        return token && token->type == type;
}

extern int c_parser_has_tokens(struct c_parser* parser, size_t count)
{
        c_parser_save_state(parser);
        while (count)
        {
                if (!c_parser_advance(parser))
                        break;
                count--;
        }
        c_parser_load_state(parser);
        return count == 0;
}

extern int c_parser_advance_if_next_token_is(struct c_parser* parser, enum c_token_type type)
{
        int res = c_parser_next_token_is(parser, type);
        if (res)
                c_parser_advance(parser);
        return res;
}

extern int c_parser_advance_if_token_is(struct c_parser* parser, enum c_token_type type)
{
        int res = c_parser_token_is(parser, type);
        if (res)
                c_parser_advance(parser);
        return res;
}

static void c_parser_handle_errs(struct c_parser* parser)
{
        c_parser_shutdown(parser);
}

extern void c_parser_init_first_token(struct c_parser* parser)
{
        c_parser_lex_token(parser);
        c_parser_token(parser) = list_head(&parser->token_list);
}
