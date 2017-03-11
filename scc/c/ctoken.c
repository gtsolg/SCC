#include "ctoken.h"

extern int c_token_is_qualifier(const c_token* token)
{
        switch (token->type)
        {
                case ctt_const:
                case ctt_volatile:
                // case ctt_restrict:
                // case ctt_atomic:
                // ...
                        return 1;

                default:
                        return 0;
        }
}

extern int c_token_is_storage_class(const c_token* token)
{
        switch (token->type)
        {
                case ctt_auto:
                case ctt_register:
                case ctt_extern:
                case ctt_static:
                        return 1;

                default:
                        return 0;
        }
}

extern int c_token_is_const_val(const c_token* token)
{
        switch (token->type)
        {
                case ctt_const_int:
                case ctt_const_float:
                case ctt_const_string:
                        return 1;

                default:
                        return 0;
        }
}

extern int c_token_is_default_type(const c_token* token)
{
        switch (token->type)
        {
                case ctt_char:
                case ctt_short:
                case ctt_int:
                case ctt_long:
                case ctt_float:
                case ctt_double:
                case ctt_signed:
                case ctt_unsigned:
                case ctt_void:
                case ctt_ellipsis:
                        return 1;

                default:
                        return 0;
        }
}

extern int c_token_is_composite_type(const c_token* token)
{
        switch (token->type)
        {
                case ctt_struct:
                case ctt_union:
                case ctt_enum:
                        return 1;

                default:
                        return 0;
        }
}

extern int c_token_is_func_call_type(const c_token* token)
{
        switch (token->type)
        {
                //case ctt_cdecl:
                //case ctt_fastcall:
                //case ctt_stdcall:
                //        return 1;

                default:
                        return 0;
        }
}

extern int c_token_is_exp_modifier(const c_token* token)
{
        return c_token_is_func_call_type(token)
                || c_token_is_storage_class(token)
                || c_token_is_qualifier(token)
                || token->type == ctt_inline;
}