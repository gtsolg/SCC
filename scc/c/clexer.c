#include "clexer.h"
#include "ccommon.h"
#include "climits.h"
#include "str_intern.h"

static inline int token_exists(const char* token, size_t len)
{
        return htab_exists(&c_reswords_map, hash64_str_len(token, len));
}

static inline enum c_token_type token_type(const char* token, size_t len)
{
        return (enum c_token_type)htab_get(&c_reswords_map, hash64_str_len(token, len));
}

static inline int wspace(int c)
{
        switch (c)
        {
                case ' ':
                case '\n':
                case '\t':
                case '\r':
                        return 1;

                default:
                        return 0;
        }
}

static inline int alpha(int c)
{
        return c >= 'a' && c <= 'z'
               || c >= 'A' && c <= 'Z'
               || c == '_';
}

static inline int digit(int c)
{
        return c >= '0' && c <= '9';
}

static inline int sign(int c)
{
        return c == '-' || c == '+';
}

static inline int hex(int c)
{
        return digit(c)
               || c >= 'a' && c <= 'f'
               || c >= 'A' && c <= 'F';
}

static inline int symb(int c)
{
        return c >= 33 && c <= 47
               || c >= 58 && c <= 64
               || c >= 91 && c <= 94
               || c == 96
               || c >= 123 && c <= 126;
}

static inline int ident(int c)
{
        return alpha(c) || digit(c);
}

static inline void skip_wspace(struct c_lexer* lexer)
{
        while(wspace(*lexer->pos))
                lexer->pos++;
        lexer->eof = !*lexer->pos;
}

static inline size_t skip_till(struct c_lexer* lexer, int c)
{
        size_t len = 0;
        while (*lexer->pos && *lexer->pos != c)
        {
                lexer->pos++;
                len++;
        }
        lexer->eof = !*lexer->pos;
        return *lexer->pos == c ? len : 0;
}

static inline void skip_to_wspace(struct c_lexer* lexer)
{
        while(*lexer->pos && !wspace(*lexer->pos))
                lexer->pos++;
        lexer->eof = !*lexer->pos;
}

static inline size_t get_sequence_length(struct c_lexer* lexer, int(*in_sequence)(int))
{
        size_t len = 0;
        while (in_sequence(*lexer->pos))
        {
                lexer->pos++;
                len++;
        }
        lexer->eof = !*lexer->pos;
        return len;
}

extern struct c_token* c_lex_ident_token(struct c_lexer* lexer)
{
        const char* begin = lexer->pos;
        size_t len = get_sequence_length(lexer, &ident);

        if (token_exists(begin, len))
                return c_token_create(token_type(begin, len), 0);
        return c_token_create(ctt_identifier, (uint64_t)str_intern_from(begin, len));
}

extern struct c_token* c_lex_cstr_token(struct c_lexer* lexer)
{
        const char* begin = ++lexer->pos;
        size_t len = skip_till(lexer, '\"');
        if (!len)
                return NULL;
        lexer->pos++;
        lexer->eof = !*lexer->pos;
        return c_token_create(ctt_const_string, (uint64_t)str_intern_from(begin, len));
}

extern struct c_token* c_lex_cchar_token(struct c_lexer* lexer)
{
        static const char escape[] = "nt0'\"";
        static const char codes[] = "\n\t\0\'\"";

        const char* begin = ++lexer->pos;
        size_t len = skip_till(lexer, '\'');
        if (len > 2)
                return NULL;
        lexer->pos++;
        lexer->eof = !*lexer->pos;
        if (len == 2)
        {
                if (*begin != '\\')
                        return NULL;
                *begin++;
                for (int i = 0; i < 5; i++)
                        if (*begin == escape[i])
                                return c_token_create(ctt_const_char, codes[i]);
        }
        else
                return c_token_create(ctt_const_char, *begin);
        return NULL;
}

static int possible_number(int c)
{
        return digit(c) || hex(c) || c == 'x' || c == 'X' || c == '.';
}

#define C_NOT_A_NUMBER 0
#define C_HEX_NUMBER   1
#define C_INT_NUMBER   2
#define C_FLT_NUMBER   3

static inline int get_numtype(const char* num)
{
        // todo: 12l 12ll 12u 12ul 12ull 12f
        num++;
        if (*num == 'x' || *num == 'X')
        {
                num++;
                while (*num)
                        if (!hex(*num++))
                                return C_NOT_A_NUMBER;
                return C_HEX_NUMBER;
        }
        int dot_count = 0;
        while (*num)
        {
                if (*num == '.')
                        dot_count++;
                else if (!digit(*num))
                        return C_NOT_A_NUMBER;
                num++;
        }
        if (dot_count > 1)
                return C_NOT_A_NUMBER;
        return dot_count ? C_FLT_NUMBER : C_INT_NUMBER;
}

extern struct c_token* c_lex_cnum_token(struct c_lexer* lexer)
{
        static char buf[C_MAX_CNUM_LENGTH];

        const char* begin = lexer->pos;
        size_t len = get_sequence_length(lexer, possible_number);
        if (len > C_MAX_CNUM_LENGTH)
                return NULL;

        memcpy(buf, begin, len);
        buf[len] = '\0';

        int type = get_numtype(buf);
        if (type == C_NOT_A_NUMBER)
                return NULL;
        else if (type == C_FLT_NUMBER)
        {
                double res = atof(buf);
                return c_token_create(ctt_const_float, *(uint64_t*)&res);
        }
        else if (type == C_HEX_NUMBER)
                return c_token_create(ctt_const_int, strtol(buf, NULL, 16));
        else
                return c_token_create(ctt_const_int, atoll(buf));
}

extern struct c_token* c_lex_op_token(struct c_lexer* lexer)
{
        const char* begin = lexer->pos;
        size_t len = get_sequence_length(lexer, &symb);

        for (size_t i = len; i > 0; i--)
                if (token_exists(begin, i))
                {
                        lexer->pos = begin + i;
                        lexer->eof = !*lexer->pos;
                        return c_token_create(token_type(begin, i), 0);
                }
        return NULL;
}

extern struct c_token* c_lex_token(struct c_lexer* lexer)
{
        if (lexer->eof)
                return NULL;
        skip_wspace(lexer);
        int c = *lexer->pos;
        if (alpha(c))
                return c_lex_ident_token(lexer);
        else if (c == '\"')
                return c_lex_cstr_token(lexer);
        else if (c == '\'')
                return c_lex_cchar_token(lexer);
        else if (symb(c))
                return c_lex_op_token(lexer);
        else if (digit(c))
                return c_lex_cnum_token(lexer);
        else
                return NULL;
}