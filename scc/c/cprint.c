#include "cprint.h"
#include "cparser.h"

static char* c_exp_format[] = 
{
        "",     // ok_null
        "%s %s",   // ok_operand
        "%S %s", // ok_attrib
        "%s++", // ok_post_inc
        "%s--", // ok_post_dec
        "{%s}", // ok_list
        "++%s", // ok_pre_inc
        "--%s", // ok_pre_dec
        "+%s",  // ok_plus
        "-%s",  // ok_minus
        "~%s",  // ok_bit_not
        "!%s",  // ok_not
        "*%s",  // ok_dereference
        "&%s",  // ok_address

        "%s(%s)",    // ok_call
        "%s[%s]",    // ok_subscript
        "%s.%s",     // ok_member
        "%s->%s",    // ok_member_ptr
        "(%S)%s",    // ok_cast
        "%s %s",     // ok_type
        "%s * %s",   // ok_mul
        "%s / %s",   // ok_div
        "%s % %s",   // ok_mod
        "%s + %s",   // ok_add
        "%s - %s",   // ok_sub
        "%s << %s",  // ok_shl
        "%s >> %s",  // ok_shr
        "%s < %s",   // ok_le
        "%s > %s",   // ok_gr
        "%s <= %s",  // ok_leq
        "%s >= %s",  // ok_geq
        "%s == %s",  // ok_eq
        "%s != %s",  // ok_neq
        "%s & %s",   // ok_bit_and
        "%s ^ %s",   // ok_bit_xor
        "%s | %s",   // ok_bit_or
        "%s && %s",  // ok_and
        "%s || %s",  // ok_or
        "%s = %s",   // ok_assign
        "%s += %s",  // ok_add_assign
        "%s -= %s",  // ok_sub_assign
        "%s *= %s",  // ok_mul_assign
        "%s /= %s",  // ok_div_assign
        "%s %= %s",  // ok_mod_assign
        "%s <<= %s", // ok_shl_assign
        "%s >>= %s", // ok_shr_assign
        "%s &= %s",  // ok_and_assing
        "%s ^= %s",  // ok_xor_assing
        "%s |= %s",  // ok_or_assing
        "%s, %s",    // ok_coma
};

static int needs_brackets(tree exp, tree prev)
{
        if (!prev)
                return 0;
        enum expr_node_kind p = tree_exp_kind(prev);
        enum expr_node_kind e = tree_exp_kind(exp);
        if (e == ok_operand || p == ok_cast)
                return 0;

        if ((p == ok_subscript || p == ok_call) && exp == tree_exp_right(prev))
                return 0;

        if (p == ok_call && !c_node_is_operand(exp))
                return 1;

        unsigned pcp = tree_exp_computed_precedence(prev);
        unsigned ecp = tree_exp_computed_precedence(exp);

        return ecp > pcp && tree_exp_precedence(exp) <= tree_exp_precedence(prev);
}

static char* node_to_str(tree, tree);

static char* exp_to_str(tree exp, tree prev)
{
        const char* c_format = c_exp_format[tree_exp_kind(exp)];
        char* left = node_to_str(tree_exp_left(exp), exp);
        char* right = node_to_str(tree_exp_right(exp), exp);
        if (!*left)
                left = right;
        char* res = format(c_format, left, right);
        if (needs_brackets(exp, prev))
                res = format("(%s)", res);
        return res;
}

extern char* c_exp_to_str(tree exp)
{
        return exp_to_str(exp, NULL);
}

static char* node_to_str(tree node, tree prev)
{
        if (!node)
                return format("");
        switch (tree_kind(node))
        {
                case tnk_stmt:         return NULL;
                case tnk_id: return format("%S"
                        , string_pool_get_string_by_ref(STD_STR_POOL, tree_id_ref(node)));

                case tnk_type_decl:    return NULL;
                case tnk_var_decl:     return NULL;
                case tnk_func_decl:    return NULL;
                case tnk_exp:          return exp_to_str(node, prev);
                case tnk_list:         return NULL;
                case tnk_list_node:    return NULL;

                case tnk_const_int:   
                case tnk_const_float: 
                case tnk_const_real:  
                case tnk_const_string:
                        return c_const_to_str(node);

                case tnk_vector_type:  return NULL;
                case tnk_sign_type:    return NULL;
                case tnk_type:         return c_type_to_str(node);
                case tnk_attrib:       return c_reswords[tree_attrib(node)];
                default:
                        return NULL;
        }
}

extern char* c_const_to_str(tree cst)
{
        char buf[C_MAX_CNUM_LENGTH] = { 0 };
        switch (tree_kind(cst))
        {
                case tnk_const_int:
                        sprintf(buf, "%d", tree_const_int(cst));
                        break;

                case tnk_const_float:
                        sprintf(buf, "%f", tree_const_float(cst));
                        break;

                case tnk_const_real:
                        sprintf(buf, "%lf", tree_const_real(cst));
                        break;

                case tnk_const_string:
                        return format("\"%S\""
                                , string_pool_get_string_by_ref(STD_STR_POOL, tree_const_strref(cst)));
        }
        return strcopy(buf);
}

extern char* c_node_to_str(tree node)
{
        return node_to_str(node, NULL);
}

extern char* c_type_to_str(tree type)
{
        switch (tree_type_kind(type))
        {
                case tk_void:   return strcopy("void");
                case tk_int8:   return strcopy("char");
                case tk_uint8:  return strcopy("unsigned char");
                case tk_int16:  return strcopy("short");
                case tk_uint16: return strcopy("unsigned short");
                case tk_int32:  return strcopy("int");
                case tk_uint32: return strcopy("unsigned");
                case tk_int64:  return strcopy("long long");
                case tk_uint64: return strcopy("unsigned long long");
                case tk_float:  return strcopy("float");
                case tk_double: return strcopy("double");

                case tk_record:
                case tk_union:
                case tk_vector:
                case tk_pointer:
                default:
                        return NULL;
        }
}