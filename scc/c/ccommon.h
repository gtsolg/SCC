#ifndef C_COMMON_H
#define C_COMMON_H

#include "htab.h"
#include "hash.h"
#include "tree/tree.h"
#include "tree/tree_print.h"

#include "tree/cprint.h"
#include "reader/ctoken.h"
#include "climits.h"

enum c_family_lang_flags
{
        lf_c     = 1 << 0,
        lf_cxx   = 1 << 1,
        lf_obj_c = 1 << 2,
};

static const struct tree_exp_info c_opinfo[] = 
{
        tree_exp_info_init(ok_operand,     oak_left_to_right,  0),
        tree_exp_info_init(ok_attrib,      oak_left_to_right,  0),
        tree_exp_info_init(ok_post_inc,    oak_left_to_right, 15),
        tree_exp_info_init(ok_post_dec,    oak_left_to_right, 15),
        tree_exp_info_init(ok_list,        oak_left_to_right, 15),
        tree_exp_info_init(ok_pre_inc,     oak_right_to_left, 14),
        tree_exp_info_init(ok_pre_dec,     oak_right_to_left, 14),
        tree_exp_info_init(ok_plus,        oak_right_to_left, 14),
        tree_exp_info_init(ok_minus,       oak_right_to_left, 14),
        tree_exp_info_init(ok_bit_not,     oak_right_to_left, 14),
        tree_exp_info_init(ok_not,         oak_right_to_left, 14),
        tree_exp_info_init(ok_dereference, oak_right_to_left, 14),
        tree_exp_info_init(ok_address,     oak_right_to_left, 14),
        tree_exp_info_init(ok_call,        oak_left_to_right, 15),
        tree_exp_info_init(ok_subscript,   oak_left_to_right, 15),
        tree_exp_info_init(ok_member,      oak_left_to_right, 15),
        tree_exp_info_init(ok_member_ptr,  oak_left_to_right, 15),
        tree_exp_info_init(ok_cast,        oak_right_to_left, 14),
        tree_exp_info_init(ok_type,        oak_right_to_left, 14),
        tree_exp_info_init(ok_mul,         oak_left_to_right, 13),
        tree_exp_info_init(ok_div,         oak_left_to_right, 13),
        tree_exp_info_init(ok_mod,         oak_left_to_right, 13),
        tree_exp_info_init(ok_add,         oak_left_to_right, 12),
        tree_exp_info_init(ok_sub,         oak_left_to_right, 12),
        tree_exp_info_init(ok_shl,         oak_left_to_right, 11),
        tree_exp_info_init(ok_shr,         oak_left_to_right, 11),
        tree_exp_info_init(ok_le,          oak_left_to_right, 10),
        tree_exp_info_init(ok_gr,          oak_left_to_right, 10),
        tree_exp_info_init(ok_leq,         oak_left_to_right, 10),
        tree_exp_info_init(ok_geq,         oak_left_to_right, 10),
        tree_exp_info_init(ok_eq,          oak_left_to_right,  9),
        tree_exp_info_init(ok_neq,         oak_left_to_right,  9),
        tree_exp_info_init(ok_bit_and,     oak_left_to_right,  8),
        tree_exp_info_init(ok_bit_xor,     oak_left_to_right,  7),
        tree_exp_info_init(ok_bit_or,      oak_left_to_right,  6),
        tree_exp_info_init(ok_and,         oak_left_to_right,  5),
        tree_exp_info_init(ok_or,          oak_left_to_right,  4),
        tree_exp_info_init(ok_assign,      oak_right_to_left,  3),
        tree_exp_info_init(ok_add_assign,  oak_right_to_left,  3),
        tree_exp_info_init(ok_sub_assign,  oak_right_to_left,  3),
        tree_exp_info_init(ok_mul_assign,  oak_right_to_left,  3),
        tree_exp_info_init(ok_div_assign,  oak_right_to_left,  3),
        tree_exp_info_init(ok_mod_assign,  oak_right_to_left,  3),
        tree_exp_info_init(ok_shl_assign,  oak_right_to_left,  3),
        tree_exp_info_init(ok_shr_assign,  oak_right_to_left,  3),
        tree_exp_info_init(ok_and_assing,  oak_right_to_left,  3),
        tree_exp_info_init(ok_xor_assing,  oak_right_to_left,  3),
        tree_exp_info_init(ok_or_assing,   oak_right_to_left,  3),
        tree_exp_info_init(ok_coma,        oak_right_to_left,  2),
};

static const char* c_reswords[] = 
{
        "", // This is used to enumerate token_type from 1

#include "creswords.inc"
};

#define C_RESWORDS_SIZE (sizeof(c_reswords) / sizeof(char*))

extern uint64_t c_reswords_hash[C_RESWORDS_SIZE];
extern struct htab c_reswords_map;
extern struct htab c_const_tree;

extern scc_err_t c_setup();

#endif // !C_COMMON_H
