#ifndef C_LEXER_H
#define C_LEXER_H

#include "c/ctoken.h"

//extern void        c_lexer_setup();
//extern uint64_t    c_resword_hash(enum c_token_type rword);
//extern const char* c_resword(enum c_token_type rword);

struct c_lexer
{
        const char* pos;
        int eof;
};

#define c_lexer_init(cstrptr) { cstrptr, 0 }

extern struct c_token* c_lex_ident_token(struct c_lexer* lexer);
extern struct c_token* c_lex_cstr_token(struct c_lexer* lexer);
extern struct c_token* c_lex_cchar_token(struct c_lexer* lexer);
extern struct c_token* c_lex_cnum_token(struct c_lexer* lexer);
extern struct c_token* c_lex_op_token(struct c_lexer* lexer);

extern struct c_token* c_lex_token(struct c_lexer* lexer);

#endif // !C_LEXER_H
