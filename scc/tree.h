#ifndef _TREE_H
#define _TREE_H

#include "list.h"
#include "str_pool.h"

typedef struct tree_node* tree;

struct tree_const_int
{
        uint64_t val;
};

struct tree_const_float
{
        float val;
};

struct tree_const_real
{
        long double val;
};

struct tree_const_string
{
        strref_t ref;
};

struct tree_vector_type
{
        tree type;
        size_t size;
};

enum type_qualifier
{
        tq_unqualified = 0 << 0,
        tq_const       = 1 << 0,
        tq_volatile    = 1 << 1,
        tq_restrict    = 1 << 2,
        tq_const_const = 1 << 3,
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
        tk_pointer,
        tk_function_pointer,
};

struct tree_type
{
        enum type_kind kind;
        enum type_qualifier qual;

        // tree_type, sign_type, vector_type or NULL
        tree type;
};

struct tree_identifier
{
        strref_t ref;
};

enum decl_scope
{
        ds_default,
        ds_extern,
        ds_static,
};

struct tree_decl
{
        enum decl_scope scope;
        tree id;

        // tree_type if type_decl or var_decl
        // sign_type if func decl
        tree base;
};

enum operator_kind
{
        ok_null,
        ok_operand,
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
        ok_and_assing,
        ok_xor_assing,
        ok_or_assing,
        ok_coma,
};

enum operator_assoc_kind
{
        oak_left_to_right,
        oak_right_to_left,
};

struct tree_exp_info
{
        enum operator_kind kind;
        enum operator_assoc_kind assoc;
        unsigned precedence;
        unsigned nesting;
};

#define tree_exp_info_init(kind, assoc, precedence) { kind, assoc, precedence, 0 } 

struct tree_exp
{
        struct tree_exp_info info;
        union
        {
                tree right;
                tree next;
        };
        union
        {
                tree left;
                tree node;
        };
};

struct tree_expr_stmt
{
        tree exp;
        tree type;
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
        sk_expr,
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
        enum statement_kind kind;
        union
        {
                struct tree_expr_stmt exp_stmt;
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
        struct list_node node;
        // any node
        tree base;
};

struct tree_list
{
        struct list_node list;
};

struct tree_func_sign_type
{
        tree restype;
        // expr statement list
        tree args;
};

enum decl_type
{
        dt_cdecl,
        dt_stdcall,
        dt_fastcall,
};

struct tree_func_decl
{
        tree decl;
        // statement list
        tree body;

        enum decl_type decl_type;
        unsigned is_inline : 1;
};

enum tree_node_kind
{
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
};

struct tree_node
{
        enum tree_node_kind kind;
        union
        {
                struct tree_stmt stmt;
                struct tree_identifier id;
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
        };
};

//static struct tree_node __null_node;
//#define TREE_NULL_NODE &__null_node

static struct tree_node __null_exp = { tnk_exp, ok_null };
#define TREE_NULL_EXP_NODE &__null_exp;

extern tree tree_create(struct allocator* alloc, enum tree_node_kind kind);
extern void tree_delete(struct allocator* alloc, tree node);

extern tree tree_type_create(struct allocator* alloc, enum type_kind kind, enum type_qualifier qual, tree type);
extern tree tree_ident_create(struct allocator* alloc, strref_t ref);
extern tree tree_list_node_create(struct allocator* alloc, tree base);
extern tree tree_exp_create(struct allocator* alloc, enum operator_kind kind, unsigned nesting, const struct tree_exp_info* opinfo);
extern tree tree_const_int_create(struct allocator* alloc, uint64_t val);
extern tree tree_const_real_create(struct allocator* alloc, long double val);
extern tree tree_const_float_create(struct allocator* alloc, float val);
extern tree tree_const_string_create(struct allocator* alloc, strref_t ref);

#define tree_kind(ptree)           (ptree)->kind
#define tree_type(ptree)           (ptree)->type.type
#define tree_type_kind(ptree)      (ptree)->type.kind
#define tree_type_qual(ptree)      (ptree)->type.qual
#define tree_list(ptree)           (ptree)->list.list
#define tree_list_node(ptree)      (ptree)->list_node.node
#define tree_list_node_base(ptree) (ptree)->list_node.base
#define tree_exp_info(ptree)       (ptree)->exp.info
#define tree_exp_kind(ptree)       tree_exp_info(ptree).kind
#define tree_exp_left(ptree)       (ptree)->exp.left
#define tree_exp_right(ptree)      (ptree)->exp.right
#define tree_exp_next(ptree)       tree_exp_right(ptree)
#define tree_exp_node(ptree)       tree_exp_left(ptree)
#define tree_exp_assoc(ptree)      tree_exp_info(ptree).assoc
#define tree_exp_precedence(ptree) tree_exp_info(ptree).precedence
#define tree_exp_nesting(ptree)    tree_exp_info(ptree).nesting
#define tree_exp_computed_precedence(ptree) (tree_exp_precedence(ptree) + tree_exp_nesting(ptree) * 100)
#define tree_id_ref(ptree)         (ptree)->id.ref
#define tree_const_strref(ptree)   (ptree)->const_string.ref
#define tree_const_float(ptree)    (ptree)->const_float.val
#define tree_const_real(ptree)     (ptree)->const_real.val
#define tree_const_int(ptree)      (ptree)->const_int.val

#define tree_list_empty(ptree) list_empty(&tree_list(ptree))

#define tree_list_node_next(ptree)\
        list_next_off(&tree_list_node(ptree), struct tree_node, list_node)

#define tree_list_node_prev(ptree)\
        list_prev_off(&tree_list_node(ptree), struct tree_node, list_node)

#define tree_list_iterator_init(ptree)    (tree_list_head(ptree))
#define tree_list_iterator_advance(ptree) (ptree = tree_list_node_next(ptree))
#define tree_list_iterator_rewind(ptree)  (ptree = tree_list_node_prev(ptree))
#define tree_list_iterator_valid(ptree)   (tree_kind(ptree) == tnk_list_node)

#define tree_list_node_remove(ptree) list_remove(&tree_list_node(ptree))

#define tree_list_node_insert_before(ptree, pnode)\
        list_add_before(&tree_list_node(ptree), &tree_list_node(pnode))

#define tree_list_node_insert_after(ptree, pnode)\
        list_add_after(&tree_list_node(ptree), &tree_list_node(pnode))

static inline struct tree_node tree_list_init(tree list)
{
        tree_kind(list) = tnk_list;
        list_initf(&tree_list(list));
        return *list;
}

#define tree_list_tail(ptree)\
        (tree)list_tail_off(&tree_list(ptree), struct tree_node, list_node)

#define tree_list_head(ptree)\
        (tree)list_head_off(&tree_list(ptree), struct tree_node, list_node)

#define tree_list_end(ptree) (ptree)

#define tree_list_pop_back(ptree)\
        (tree)list_pop_back_off(&tree_list(ptree), struct tree_node, list_node)

#define tree_list_pop_front(ptree)\
        (tree)list_pop_front_off(&tree_list(ptree), struct tree_node, list_node)

#define tree_list_push_back(ptree, pnode)\
        list_push_back(&tree_list(ptree), &tree_list_node(pnode))

#define tree_list_push_front(ptree, pnode)\
        list_push_front(&tree_list(ptree), &tree_list_node(pnode))

#endif // !_TREE_H
