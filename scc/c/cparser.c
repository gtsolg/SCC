#include "cparser.h"
#include "ccommon.h"

//struct c_parser_state
//{
//        struct list_node node;
//        struct c_token* token;
//        size_t nesting;
//        size_t counter;
//};

extern void c_parser_init(struct c_parser* parser, const char* str)
{
        struct c_lexer lexer = c_lexer_init(str);
        //list_init(&parser->states);
        list_init(&parser->token_list);
        c_parser_lexer(parser) = lexer;
        parser->state_idx = 0;
        c_parser_counter(parser) = 0;
        c_parser_nesting(parser) = 0;
        c_parser_lex_token(parser);
        c_parser_token(parser) = list_head(&parser->token_list);
        c_parser_disable_nesting_tracking(parser);
}

extern void c_parser_lex_token(struct c_parser* parser)
{
        struct c_token* token = c_lex_token(&parser->lexer);
        if (!token)
                return;
        list_push_back(&parser->token_list, token);
}

extern void c_parser_save_state(struct c_parser* parser)
{
        if (parser->state_idx >= C_PARSER_MAX_STATES)
        {
                // expression is too complex
                assert(0);
                return;
        }
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
        //if (!c_parser_next_token(parser))
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
        while(c_parser_advance(parser))
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
        struct c_token* token = c_parser_next_token(parser);
        return token->type == type;
}

extern int c_parser_prev_token_is(struct c_parser* parser, enum c_token_type type)
{
        struct c_token* token = c_parser_prev_token(parser);
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

#include "cexpr.h"

extern struct c_symtab* c_parse(struct c_parser* parser)
{
}