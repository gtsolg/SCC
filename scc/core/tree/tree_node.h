#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "list.h"
#include "str_pool.h"

typedef union tree_node* tree;

enum tree_node_kind
{
        tnk_null,
        tnk_stmt,
        tnk_id,
        tnk_type_decl,
        tnk_var_decl,
        tnk_func_decl,
        tnk_exp,
        tnk_list,
        tnk_list_node,
        tnk_const_int,
        tnk_const_float,
        tnk_const_real,
        tnk_const_string,
        tnk_vector_type,
        tnk_sign_type,
        tnk_type,
        tnk_attrib,

        tnk_size,
};

// this structure must be the first member of any struct related to tree
struct tree_base
{
        enum tree_node_kind kind;
};

struct tree_const_int
{
        struct tree_base base;
        uint64_t val;
};

struct tree_const_float
{
        struct tree_base base;
        float val;
};

struct tree_const_real
{
        struct tree_base base;
        long double val;
};

struct tree_const_string
{
        struct tree_base base;
        strref_t ref;
};

struct tree_vector_type
{
        struct tree_base base;
        tree type;
        uint64_t size;
};

enum type_qualifier
{
        tq_unqualified = 0 << 0,
        tq_const       = 1 << 0,
        tq_volatile    = 1 << 1,
        tq_restrict    = 1 << 2,
};

enum type_kind
{
        tk_void,
        tk_int8,
        tk_uint8,
        tk_int16,
        tk_uint16,
        tk_int32,
        tk_uint32,
        tk_int64,
        tk_uint64,
        tk_float,
        tk_double,
        tk_record,
        tk_union,
        tk_vector,
        tk_sign,
        tk_pointer,
        tk_va_arg,
        tk_declared, // this is used when we want to add type qualifiers to declared type
};

struct tree_type
{
        struct tree_base base;

        enum type_kind kind;
        enum type_qualifier qual;

        // tree_type, sign_type, type_decl, vector_type or NULL
        tree next;
};

struct tree_identifier
{
        struct tree_base base;

        strref_t ref;
};

enum decl_storage_class
{
        dsc_auto,
        dsc_register,
        dsc_extern,
        dsc_static,
};

struct tree_decl
{
        struct tree_base tbase;

        enum decl_storage_class storage_class;
        tree id;
        // tree_type if type_decl or var_decl
        // sign_type if func decl
        tree base;
};

enum expr_node_kind
{
        ok_operand, // a leaf in an expression tree, can only point to ok_attribute
        ok_attrib, // holds modifiers like const, __cdecl, inline, ...

        // unary
        ok_post_inc,
        ok_post_dec,
        ok_list,
        ok_pre_inc,
        ok_pre_dec,
        ok_plus,
        ok_minus,
        ok_bit_not,
        ok_not,
        ok_dereference,
        ok_address,
        // binary
        ok_call,
        ok_subscript,
        ok_member,
        ok_member_ptr,
        ok_cast,
        ok_type,
        ok_mul,
        ok_div,
        ok_mod,
        ok_add,
        ok_sub,
        ok_shl,
        ok_shr,
        ok_le,
        ok_gr,
        ok_leq,
        ok_geq,
        ok_eq,
        ok_neq,
        ok_bit_and,
        ok_bit_xor,
        ok_bit_or,
        ok_and,
        ok_or,
        ok_assign,
        ok_add_assign,
        ok_sub_assign,
        ok_mul_assign,
        ok_div_assign,
        ok_mod_assign,
        ok_shl_assign,
        ok_shr_assign,
        ok_and_assign,
        ok_xor_assign,
        ok_or_assign,
        ok_coma,

        ok_size,
};

enum operator_assoc_kind
{
        oak_left_to_right,
        oak_right_to_left,
};

struct tree_exp_info
{
        enum expr_node_kind kind;
        enum operator_assoc_kind assoc;
        unsigned precedence;
        unsigned nesting;
};

#define tree_exp_info_init(kind, assoc, precedence) { kind, assoc, precedence, 0 } 

struct tree_exp
{
        struct tree_base base;
        struct tree_exp_info info;
        tree prev;
        tree right;
        tree left;
};

struct tree_expr_stmt
{
        // an expression, may contain decls
        tree base;

        tree decls;
};

struct tree_for_stmt
{
        tree init;
        tree condition;
        tree step;
};

struct tree_conditional_stmt
{
        tree condition;
        tree block;
        tree elif;
};

enum statement_kind
{
        sk_exp,
        sk_if,
        sk_while,
        sk_do_while,
        sk_for,
        sk_goto,
        sk_block,
        sk_return,
        sk_break,
        sk_continue,
};

struct tree_stmt
{
        struct tree_base base;
        enum statement_kind kind;
        union
        {
                struct tree_expr_stmt exp_stmt;
                struct tree_for_stmt for_stmt;
                struct tree_conditional_stmt while_stmt;
                struct tree_conditional_stmt do_while_stmt;
                struct tree_conditional_stmt if_stmt;
                tree label_stmt;
                tree goto_stmt;
                tree return_stmt;
                tree block_stmt;
        };
};

struct tree_list_node
{
        struct tree_base tbase;
        struct list_node node;
        tree base;
};

struct tree_list
{
        struct tree_base base;
        struct list_node list;
};

struct tree_func_sign_type
{
        struct tree_base base;
        tree restype;
        // expr statement list
        tree args;
};

enum func_call_type
{
        fct_auto,
        fct_cdecl,
        fct_stdcall,
        fct_fastcall,
};

struct tree_func_decl
{
        struct tree_base base;
        tree decl;
        tree body;

        enum func_call_type call_type;
        unsigned should_inline : 1;
};

union tree_node
{
        struct tree_stmt stmt;
        struct tree_identifier id;
        struct tree_decl decl;
        struct tree_decl type_decl;
        struct tree_decl var_decl;
        struct tree_func_decl func_decl;
        struct tree_exp exp;
        struct tree_list list;
        struct tree_list_node list_node;
        struct tree_const_int const_int;
        struct tree_const_float const_float;
        struct tree_const_real const_real;
        struct tree_const_string const_string;
        struct tree_vector_type vector_type;
        struct tree_func_sign_type sign_type;
        struct tree_type type;
        struct tree_const_int attrib;
};

#endif // !TREE_NODE_H
