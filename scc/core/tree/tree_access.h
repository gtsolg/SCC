#ifndef TREE_ACCESS_H
#define TREE_ACCESS_H

#include "tree_node.h"

#define tree_base(ptree)           ((struct tree_base*)(ptree))
#define tree_kind(ptree)           (tree_base(ptree)->kind)
#define tree_is(ptree, kind)       (tree_kind(ptree) == kind)

#define __tree(ptree)              ((tree)(ptree))
#define __tree_stmt(ptree)         __tree(ptree)->stmt
#define __tree_list(ptree)         __tree(ptree)->list.list
#define __tree_list_node(ptree)    __tree(ptree)->list_node.node

#define tree_stmt_kind(ptree)      __tree_stmt(ptree).kind
#define tree_exp_stmt_decls(ptree) __tree_stmt(ptree).exp_stmt.decls
#define tree_exp_stmt_base(ptree)  __tree_stmt(ptree).exp_stmt.base
#define tree_type_next(ptree)      __tree(ptree)->type.next
#define tree_type_kind(ptree)      __tree(ptree)->type.kind
#define tree_type_qual(ptree)      __tree(ptree)->type.qual
#define tree_list_node_base(ptree) __tree(ptree)->list_node.base
#define tree_decl_id(ptree)        __tree(ptree)->decl.id
#define tree_decl_base(ptree)      __tree(ptree)->decl.base
#define tree_exp_info(ptree)       __tree(ptree)->exp.info
#define tree_exp_kind(ptree)       tree_exp_info(ptree).kind
#define tree_exp_left(ptree)       __tree(ptree)->exp.left
#define tree_exp_right(ptree)      __tree(ptree)->exp.right
#define tree_exp_prev(ptree)       __tree(ptree)->exp.prev
#define tree_exp_next(ptree)       tree_exp_right(ptree)
#define tree_exp_node(ptree)       tree_exp_left(ptree)
#define tree_exp_assoc(ptree)      tree_exp_info(ptree).assoc
#define tree_exp_precedence(ptree) tree_exp_info(ptree).precedence
#define tree_exp_nesting(ptree)    tree_exp_info(ptree).nesting
#define tree_exp_is(ptree, kind)   (tree_exp_kind(ptree) == (kind))
#define tree_exp_computed_precedence(ptree) (tree_exp_precedence(ptree) + tree_exp_nesting(ptree) * 100)
#define tree_id_ref(ptree)         __tree(ptree)->id.ref
#define tree_const_strref(ptree)   __tree(ptree)->const_string.ref
#define tree_const_float(ptree)    __tree(ptree)->const_float.val
#define tree_const_real(ptree)     __tree(ptree)->const_real.val
#define tree_const_int(ptree)      __tree(ptree)->const_int.val
#define tree_attrib(ptree)         tree_const_int(ptree)
#define tree_vector(ptree)         __tree(ptree)->vector_type
#define tree_vector_size(ptree)    tree_vector(ptree).size
#define tree_vector_type(ptree)    tree_vector(ptree).type
#define tree_sign(ptree)           __tree(ptree)->sign_type
#define tree_sign_restype(ptree)   tree_sign(ptree).restype
#define tree_sign_args(ptree)      tree_sign(ptree).args

#define tree_list_empty(ptree) list_empty(&__tree_list(ptree))

#define tree_list_node_next(ptree)\
        list_next_off(&__tree_list_node(ptree), struct tree_list_node, node)

#define tree_list_node_prev(ptree)\
        list_prev_off(&__tree_list_node(ptree), struct tree_list_node, node)

#define tree_list_node_remove(ptree) list_remove(&__tree_list_node(ptree))

#define tree_list_node_insert_before(ptree, pnode)\
        list_add_before(&__tree_list_node(ptree), &__tree_list_node(pnode))

#define tree_list_node_insert_after(ptree, pnode)\
        list_add_after(&__tree_list_node(ptree), &__tree_list_node(pnode))

#define tree_list_tail(ptree)\
        (tree)list_tail_off(&__tree_list(ptree), struct tree_list_node, node)

#define tree_list_head(ptree)\
        (tree)list_head_off(&__tree_list(ptree), struct tree_list_node, node)

#define tree_list_end(ptree) __tree(ptree)

#define tree_list_pop_back(ptree)\
        (tree)list_pop_back_off(&__tree_list(ptree), struct tree_list_node, node)

#define tree_list_pop_front(ptree)\
        (tree)list_pop_front_off(&__tree_list(ptree), struct tree_list_node, node)

#define tree_list_push_back(ptree, pnode)\
        list_push_back(&__tree_list(ptree), &__tree_list_node(pnode))

// allocates tree_list_node
#define tree_list_push_back_node(ptree, palloc, pnode) \
        tree_list_push_back(ptree, tree_list_node_create(palloc, pnode))

#define tree_list_push_front(ptree, pnode)\
        list_push_front(&__tree_list(ptree), &__tree_list_node(pnode))

#endif // !TREE_ACCESS_H
