#include "char_info.h"

const int char_info[256] = 
{
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
//                  tab         \n
        ck_unknown, ck_wspace,  ck_newline, ck_unknown,
//                  \r
        ck_unknown, ck_newline, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
//      space       !           "           #
        ck_wspace,  ck_symbol,  ck_symbol,  ck_symbol,
//      $           %           &           '
        ck_symbol,  ck_symbol,  ck_symbol,  ck_symbol,
//      (           )           *           +
        ck_symbol,  ck_symbol,  ck_symbol,  ck_symbol,
//      ,           -           .           /
        ck_symbol,  ck_symbol,  ck_symbol,  ck_symbol,
//      0           1           2           3
        ck_digit,   ck_digit,   ck_digit,   ck_digit,
//      4           5           6           7
        ck_digit,   ck_digit,   ck_digit,   ck_digit,
//      8           9           :           ;
        ck_digit,   ck_digit,   ck_symbol,  ck_symbol,
//      <           =           >           ?
        ck_symbol,  ck_symbol,  ck_symbol,  ck_symbol,
//      @           A           B           C...
        ck_symbol,  ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
//      ...X        Y           Z           [
        ck_alpha,   ck_alpha,   ck_alpha,   ck_symbol,
//      \           ]           ^           _
        ck_symbol,  ck_symbol,  ck_symbol,  ck_alpha,
//      `           a           b           c...
        ck_symbol,  ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
        ck_alpha,   ck_alpha,   ck_alpha,   ck_alpha,
//      ...x        y           z           {
        ck_alpha,   ck_alpha,   ck_alpha,   ck_symbol,
//      |           }           ~
        ck_symbol,  ck_symbol,  ck_symbol,  ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
        ck_unknown, ck_unknown, ck_unknown, ck_unknown,
};