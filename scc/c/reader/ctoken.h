#ifndef C_TOKEN_H
#define C_TOKEN_H

#include "token.h"

typedef struct token c_token;

#define c_token_type(ptoken) (ptoken)->type
#define c_token_ref(ptoken)  (ptoken)->ref
#define c_token_next(ptoken) list_node_next(&(ptoken)->node)
#define c_token_prev(ptoken) list_node_prev(&(ptoken)->node)

enum c_token_type
{
#include "ctoken_type.inc"
};

extern int c_token_is_qualifier(const c_token* token);
extern int c_token_is_storage_class(const c_token* token);
extern int c_token_is_const_val(const c_token* token);
extern int c_token_is_default_type(const c_token* token);
extern int c_token_is_composite_type(const c_token* token);
extern int c_token_is_func_call_type(const c_token* token);
extern int c_token_is_exp_modifier(const c_token* token);

#endif // !C_TOKEN_H
