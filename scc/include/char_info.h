#ifndef CHAR_INFO_H
#define CHAR_INFO_H

#include "scc.h"

#define scc_assert_char(c) scc_assert((c) >= 0 && (c) <= 255)

extern const int char_info[256];

enum char_kind
{
        ck_unknown,
        ck_alpha   = 1 << 0, // a-z A-Z _
        ck_digit   = 1 << 1, // 0-9
        ck_symbol  = 1 << 2, //
        ck_newline = 1 << 3, // \n \r
        ck_wspace  = 1 << 4, // ' ' \t
};

static inline int get_char(int c)
{
        scc_assert_char(c);
        return char_info[c];
}

static inline int char_is_unknown(int c)
{
        return !get_char(c);
}

static inline int char_is_alpha(int c)
{
        return get_char(c) & ck_alpha;
}

static inline int char_is_digit(int c)
{
        return get_char(c) & ck_digit;
}

static inline int char_is_symbol(int c)
{
        return get_char(c) & ck_symbol;
}

static inline int char_is_newline(int c)
{
        return get_char(c) & ck_newline;
}

static inline int char_is_wspace(int c)
{
        return get_char(c) & ck_wspace;
}

#endif // !CHAR_INFO_H
