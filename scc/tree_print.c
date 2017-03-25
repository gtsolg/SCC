#include "tree_print.h"
#include "str.h"

static void tree_print_pretty(tree, char*, int);

static char* exp_kind_string[] =
{
        "operand",  // ok_operand
        "attrib",   // ok_attrib
        "post inc", // ok_post_inc
        "post dec", // ok_post_dec
        "list",     // ok_list
        "pre inc",  // ok_pre_inc
        "pre dec",  // ok_pre_dec
        "plus",     // ok_plus
        "minus",    // ok_minus
        "bit not",  // ok_bit_not
        "not",      // ok_not
        "deref",    // ok_dereference
        "addr",     // ok_address

        "call",       // ok_call
        "subs",       // ok_subscript
        "member",     // ok_member
        "member ptr", // ok_member_ptr
        "cast",       // ok_cast
        "type",       // ok_type
        "mul",        // ok_mul
        "div",        // ok_div
        "mod",        // ok_mod
        "add",        // ok_add
        "sub",        // ok_sub
        "shl",        // ok_shl
        "shr",        // ok_shr
        "le",         // ok_le
        "gr",         // ok_gr
        "leq",        // ok_leq
        "geq",        // ok_geq
        "eq",         // ok_eq
        "neq",        // ok_neq
        "bit and",    // ok_bit_and
        "bit xor",    // ok_bit_xor
        "bit or",     // ok_bit_or
        "and",        // ok_and
        "or",         // ok_or
        "assign",     // ok_assign
        "add assign", // ok_add_assign
        "sub assign", // ok_sub_assign
        "mul assign", // ok_mul_assign
        "div assign", // ok_div_assign
        "mod assign", // ok_mod_assign
        "shl assign", // ok_shl_assign
        "shr assign", // ok_shr_assign
        "and assign", // ok_and_assing
        "xor assign", // ok_xor_assing
        "or assign",  // ok_or_assing
        "coma",       // ok_coma
};

static void tree_exp_print_pretty(tree exp, char* indent, int last)
{
        printf("exp: %s\n", exp_kind_string[tree_exp_kind(exp)]);
        tree_print_pretty(tree_exp_right(exp), format("%S", indent), 0);
        tree_print_pretty(tree_exp_left(exp), format("%S", indent), 1);
}

static void print_string(strref_t ref)
{
        printf("\"%s\"\n", string_pool_get_string_by_ref(STD_STR_POOL, ref));
}

static char* type_kind_string[] = 
{
        "void",    // tk_void
        "int8",    // tk_int8
        "uint8",   // tk_uint8
        "int16",   // tk_int16
        "uint16",  // tk_uint16
        "int32",   // tk_int32
        "uint32",  // tk_uint32
        "int64",   // tk_int64
        "uint64",  // tk_uint64
        "float",   // tk_float
        "double",  // tk_double
        "record",  // tk_record
        "union",   // tk_union
        "vector",  // tk_vector
        "pointer", // tk_pointer
        "va_arg",  // tk_va_arg
};

static void tree_print_type(tree type)
{
        enum type_qualifier qual = tree_type_qual(type);
        if (qual & tq_const)
                printf("const ");
        if (qual & tq_volatile)
                printf("volatile ");
        if (qual & tq_restrict)
                printf("restrict ");

        enum type_kind kind = tree_type_kind(type);
        printf("%s", type_kind_string[kind]);

        if (kind == tk_vector)
                printf("[%llu]", tree_vector_size(tree_type(type)));
}

static void tree_print_type_pretty(tree type, char* indent, int last)
{
        tree_print_type(type);
        printf("\n");
        tree_print_pretty(tree_type(type), indent, 1);
}

static void tree_print_pretty(tree node, char* indent, int last)
{
        if (!node)
                return;
        printf("%s", indent);
        if (last)
        {
                printf("\\-");
                indent = format("%s  ", indent);
        }
        else
        {
                printf("|-");
                indent = format("%s| ", indent);
        }

        switch (tree_kind(node))
        {
                case tnk_null:         printf("TREE_NULL\n"); break;
                case tnk_stmt:         break;
                case tnk_id: print_string(tree_id_ref(node)); break;

                case tnk_type_decl:    break;
                case tnk_var_decl:     break;
                case tnk_func_decl:    break;
                case tnk_exp:          tree_exp_print_pretty(node, indent, last); break;
                case tnk_list:         break;
                case tnk_list_node:    break;

                case tnk_const_int:    printf("%llu\n", tree_const_int(node)); break;
                case tnk_const_float:  printf("%f\n", tree_const_float(node)); break;
                case tnk_const_real:   printf("%.10lf\n", tree_const_real(node)); break;
                case tnk_const_string: print_string(tree_const_strref(node)); break;

                case tnk_vector_type:  tree_print_type_pretty(tree_vector_type(node), indent, 1); break;
                case tnk_sign_type:
                case tnk_type:         tree_print_type_pretty(node, indent, last); break;

                case tnk_attrib:       printf("%llu\n", tree_attrib(node)); break;

                default:
                        printf("<UNKNOWN TREE>");
                        break;
        }
}

extern void tree_print(tree node)
{
        printf("\n");
        tree_print_pretty(node, format(""), 1);
}