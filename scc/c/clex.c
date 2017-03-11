#include "cparser.h"

#define C_MAX_OPTOKEN_LEN 3

static inline enum c_token_type token_type(const char* token, int len)
{
        return (enum c_token_type)htab_get(&c_reswords_map, hash64_str_len(token, len));
}

static inline void add_token(struct c_parser* parser, struct token* token, unsigned lang_flags)
{
        token->lang_flags = lang_flags;
        list_push_back(&parser->token_list, &token->node);
}

static void c_parser_lex_optoken(struct c_parser* parser, struct token* token
        , const char* string, int len)
{
        for (int i = 0; i < len; i++)
        {
                struct location loc = token->loc;
                loc.off += i;

                enum c_token_type type;
                if (i + 1 < len)
                {
                        type = token_type(string + i, 2);
                        if (type)
                        {
                                add_token(parser, token_create_loc(&c_parser_c_token_alloc(parser), type, loc), lf_c);
                                i++;
                                continue;
                        }
                }

                type = token_type(string + i, 1);
                if (!type)
                        c_parser_handle_err(parser, SCC_ERR); // unknown token
                add_token(parser, token_create_loc(&c_parser_c_token_alloc(parser), type, loc), lf_c);
        }
        token_delete(&c_parser_c_token_alloc(parser), token);
}

static void c_parser_lex_ident_token(struct c_parser* parser, struct token* token
        , const char* string, int len)
{
        enum c_token_type type = token_type(string, len);
        if (!type)
        {
                type = ctt_identifier;
                token->ref = string_pool(STD_STR_POOL, string, len);
        }
        token->type = type;
        add_token(parser, token, lf_c);
}

static void c_parser_lex_cst_num_token(struct c_parser* parser, struct token* token
        , const char* string, int len)
{
        token->type = ctt_const_int;
        char num[100] = { 0 };
        memcpy(num, string, len);
        token->integer = atoll(num);
        add_token(parser, token, lf_c);
}

static void c_parser_lex_cst_str_token(struct c_parser* parser, struct token* token
        , const char* string, int len)
{
        token->type = ctt_const_string;
        token->ref = string_pool(STD_STR_POOL, string, len);
        add_token(parser, token, lf_c);
}

static int escape_to_char(int c)
{
        switch (c)
        {
                case 'n': return '\n';
                case 't': return '\t';
                case '0': return '\0';

                default:
                        return c;
        }
}

static void c_parser_lex_cst_char_token(struct c_parser* parser, struct token* token
        , const char* string, int len)
{
        if (len > 2 || len == 2 && *string != '\\')
                c_parser_handle_err(parser, SCC_ERR); // incorrect character

        token->integer = len == 1 ? *string : escape_to_char(*++string);
        token->type = ctt_const_int;
        add_token(parser, token, lf_c);
}

const static void(*lex_dispatch_table[crtt_size])(struct c_parser*, struct token*, char*, int) =
{
        NULL, // crtt_eof
        NULL, // crtt_new_line
        NULL, // crtt_whitespace
        c_parser_lex_ident_token,    // crtt_word
        c_parser_lex_cst_str_token,  // crtt_cst_str
        c_parser_lex_cst_char_token, // crtt_cst_char
        c_parser_lex_cst_num_token,  // crtt_numbers
        c_parser_lex_optoken,        // crtt_symbols
};

extern void c_parser_lex_token(struct c_parser* parser)
{
        while (!c_parser_reader(parser)->eof)
        {
                struct token* token = c_read_token(c_parser_reader(parser));
                c_parser_error_fence(parser);
                if (!token)
                {
                        add_token(parser, token_create(&c_parser_c_token_alloc(parser), ctt_eof), lf_c);
                        return;
                }

                enum c_reader_token_type type = token->type;

                if (type == crtt_new_line || type == crtt_whitespace)
                        token_delete(&c_parser_c_token_alloc(parser), token);
                else
                {
                        char* string = NULL;
                        int len = 0;
                        c_reader_loc_expose(c_parser_reader(parser), token->integer, &string, &len);
                        lex_dispatch_table[token->type](parser, token, string, len);
                        return;
                }
        }
}
