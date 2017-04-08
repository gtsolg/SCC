#include "ctree_print.h"
#include "../parse/cparser.h"
#include "../ccommon.h"
#include "str.h"

static char* c_exp_format[] =
{
        "%s %s", // ok_operand
        "%s %s", // ok_attrib
        "%s++",  // ok_post_inc
        "%s--",  // ok_post_dec
        "{%s}",  // ok_list
        "++%s",  // ok_pre_inc
        "--%s",  // ok_pre_dec
        "+%s",   // ok_plus
        "-%s",   // ok_minus
        "~%s",   // ok_bit_not
        "!%s",   // ok_not
        "*%s",   // ok_dereference
        "&%s",   // ok_address

        "%s(%s)",    // ok_call
        "%s[%s]",    // ok_subscript
        "%s.%s",     // ok_member
        "%s->%s",    // ok_member_ptr
        "(%s)%s",    // ok_cast
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

static void type_qual_to_str(char* buf, enum type_qualifier qual)
{
        if (qual & tq_volatile)
                strcat(buf, "volatile ");
        if (qual & tq_restrict)
                strcat(buf, "restrict ");
        if (qual & tq_const)
                strcat(buf, "const ");
}

extern void c_tree_to_str(char* buf, tree node, tree prev)
{
        switch (tree_kind(node))
        {
                case tnk_null: return;
                case tnk_stmt: return;
                case tnk_id: strcat(buf,
                        string_pool_get_string_by_ref(STD_STR_POOL, tree_id_ref(node)));
                        return;

                case tnk_type_decl:    return;
                case tnk_var_decl:     return;
                case tnk_func_decl:    return;
                case tnk_exp:          c_exp_to_str(buf, node, prev); return;
                case tnk_list:         return;
                case tnk_list_node:    return;

                case tnk_const_int:
                case tnk_const_float:
                case tnk_const_real:
                case tnk_const_string: c_cst_to_str(buf, node); return;

                case tnk_type: c_type_to_str(buf, node); return;
                case tnk_attrib: strcat(buf, c_reswords[tree_attrib(node)]); return;
                default: return;
        }
}

extern void c_cst_to_str(char* buf, tree cst)
{
        char tmp[C_MAX_LINE_LENGTH] = { 0 };
        switch (tree_kind(cst))
        {
                case tnk_const_int:    sprintf(tmp, "%d", tree_const_int(cst)); break;
                case tnk_const_float:  sprintf(tmp, "%f", tree_const_float(cst)); break;
                case tnk_const_real:   sprintf(tmp, "%lf", tree_const_real(cst)); break;
                case tnk_const_string: sprintf(tmp, "\"%s\""
                        , string_pool_get_string_by_ref(STD_STR_POOL, tree_const_strref(cst))); break;
                default: break;
        }
        strcat(buf, tmp);
}

extern void c_exp_to_str(char* buf, tree exp, tree prev)
{
        char lbuf[C_PRINT_EXP_BUF_SIZE] = { 0 };
        char rbuf[C_PRINT_EXP_BUF_SIZE] = { 0 };
        const char* c_format = c_exp_format[tree_exp_kind(exp)];

        c_tree_to_str(lbuf, tree_exp_left(exp), exp);
        c_tree_to_str(rbuf, tree_exp_right(exp), exp);
        sprintf(buf, c_format, *lbuf ? lbuf : rbuf, rbuf);
        if (needs_brackets(exp, prev))
                strwrap("(", buf, ")");
}

static void pointer_to_str(char* buf, const char* quals, tree ptr)
{
        strprecatn(buf, 2, quals, "*");
        enum type_kind kind = tree_type_kind(tree_type(ptr));
        if (kind == tk_vector || kind == tk_sign)
                strwrap("(", buf, ")");
        c_type_to_str(buf, tree_type(ptr));
}

static vector_to_str(char* buf, const char* quals, tree vec)
{
        vec = tree_type(vec);
        sprintf(buf, "%s[%llu]", buf, tree_vector_size(vec));
        c_type_to_str(buf, tree_vector_type(vec));
}

static sign_to_str(char* buf, const char* quals, tree sign)
{
        sign = tree_type(sign);
        strcat(buf, "(");
        if (!tree_is(tree_sign_args(sign), tnk_null))
        {
                struct tree_iterator it = tree_list_iterator_init(tree_sign_args(sign));
                while (tree_list_iterator_valid(&it))
                {
                        c_type_to_str(strend(buf), tree_list_iterator_node(&it));
                        tree_list_iterator_advance(&it);
                        if (tree_list_iterator_valid(&it))
                                strcat(buf, ", ");
                }
        }
        strcat(buf, ")");
        c_type_to_str(buf, tree_sign_restype(sign));
}

extern void c_type_to_str(char* buf, tree type)
{
        if (tree_is(type, tnk_null))
                return;

        char quals[40] = { 0 };
        type_qual_to_str(quals, tree_type_qual(type));

        switch (tree_type_kind(type))
        {
                case tk_void:   strprecatn(buf, 2, "void", quals); return;
                case tk_int8:   strprecatn(buf, 2, "char", quals); return;
                case tk_uint8:  strprecatn(buf, 2, "unsigned char", quals); return;
                case tk_int16:  strprecatn(buf, 2, "short", quals); return;
                case tk_uint16: strprecatn(buf, 2, "unsigned short", quals); return;
                case tk_int32:  strprecatn(buf, 2, "int", quals); return;
                case tk_uint32: strprecatn(buf, 2, "unsigned", quals); return;
                case tk_int64:  strprecatn(buf, 2, "long long", quals); return;
                case tk_uint64: strprecatn(buf, 2, "unsigned long long", quals); return;
                case tk_float:  strprecatn(buf, 2, "float", quals); return;
                case tk_double: strprecatn(buf, 2, "double", quals); return;

                case tk_sign:    sign_to_str(buf, quals, type); return;
                case tk_vector:  vector_to_str(buf, quals, type); return;
                case tk_pointer: pointer_to_str(buf, quals, type); return;

                default: return;
        }
}