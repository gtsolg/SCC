#include "tree_info.h"


scc_static_assert(ok_size == 49, "tree_info: should update tree_exp_info");

const int tree_exp_info[ok_size] = 
{
        tei_operand,                                // ok_operand
        tei_operand,                                // ok_attrib
                                      
        tei_unary | tei_left_assoc | tei_postfix,   // ok_post_inc
        tei_unary | tei_left_assoc | tei_postfix,   // ok_post_dec
        tei_unary | tei_left_assoc,                 // ok_list
        tei_unary | tei_right_assoc | tei_prefix,   // ok_pre_inc
        tei_unary | tei_right_assoc | tei_prefix,   // ok_pre_dec
        tei_unary | tei_right_assoc | tei_prefix,   // ok_plus
        tei_unary | tei_right_assoc | tei_prefix,   // ok_minus
        tei_unary | tei_right_assoc | tei_prefix,   // ok_bit_not
        tei_unary | tei_right_assoc | tei_prefix,   // ok_not
        tei_unary | tei_right_assoc | tei_prefix,   // ok_dereference
        tei_unary | tei_right_assoc | tei_prefix,   // ok_address
                                      
        tei_binary | tei_left_assoc | tei_postfix,  // ok_call
        tei_binary | tei_left_assoc | tei_postfix,  // ok_subscript
        tei_binary | tei_left_assoc,                // ok_member
        tei_binary | tei_left_assoc,                // ok_member_ptr
        tei_binary | tei_right_assoc,               // ok_cast
        tei_binary | tei_right_assoc,               // ok_type
        tei_binary | tei_left_assoc,                // ok_mul
        tei_binary | tei_left_assoc,                // ok_div
        tei_binary | tei_left_assoc,                // ok_mod
        tei_binary | tei_left_assoc,                // ok_add
        tei_binary | tei_left_assoc,                // ok_sub
        tei_binary | tei_left_assoc,                // ok_shl
        tei_binary | tei_left_assoc,                // ok_shr
        tei_binary | tei_left_assoc,                // ok_le
        tei_binary | tei_left_assoc,                // ok_gr
        tei_binary | tei_left_assoc,                // ok_leq
        tei_binary | tei_left_assoc,                // ok_geq
        tei_binary | tei_left_assoc,                // ok_eq
        tei_binary | tei_left_assoc,                // ok_neq
        tei_binary | tei_left_assoc,                // ok_bit_and
        tei_binary | tei_left_assoc,                // ok_bit_xor
        tei_binary | tei_left_assoc,                // ok_bit_or
        tei_binary | tei_left_assoc,                // ok_and
        tei_binary | tei_left_assoc,                // ok_or
        tei_binary | tei_right_assoc | tei_assign,  // ok_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_add_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_sub_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_mul_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_div_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_mod_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_shl_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_shr_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_and_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_xor_assign
        tei_binary | tei_right_assoc | tei_assign,  // ok_or_assign
        tei_binary | tei_right_assoc,               // ok_coma
};

extern const int tree_exp_precedence_info[ok_size] = 
{
        0, // ok_operand
        0, // ok_attrib

        15, // ok_post_inc
        15, // ok_post_dec
        15, // ok_list
        14, // ok_pre_inc
        14, // ok_pre_dec
        14, // ok_plus
        14, // ok_minus
        14, // ok_bit_not
        14, // ok_not
        14, // ok_dereference
        14, // ok_address

        15, // ok_call
        15, // ok_subscript
        15, // ok_member
        15, // ok_member_ptr
        14, // ok_cast
        14, // ok_type
        13, // ok_mul
        13, // ok_div
        13, // ok_mod
        12, // ok_add
        12, // ok_sub
        11, // ok_shl
        11, // ok_shr
        10, // ok_le
        10, // ok_gr
        10, // ok_leq
        10, // ok_geq
         9, // ok_eq
         9, // ok_neq
         8, // ok_bit_and
         7, // ok_bit_xor
         6, // ok_bit_or
         5, // ok_and
         4, // ok_or
         3, // ok_assign
         3, // ok_add_assign
         3, // ok_sub_assign
         3, // ok_mul_assign
         3, // ok_div_assign
         3, // ok_mod_assign
         3, // ok_shl_assign
         3, // ok_shr_assign
         3, // ok_and_assign
         3, // ok_xor_assign
         3, // ok_or_assign
         2, // ok_coma
};