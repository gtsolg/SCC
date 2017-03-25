#ifndef C_TREE_H
#define C_TREE_H

#include "ccommon.h"

extern enum type_qualifier ctt_to_qualifier(enum c_token_type type);
extern enum func_call_type ctt_to_call_type(enum c_token_type type);
extern enum decl_storage_class ctt_to_storage_class(enum c_token_type type);

extern enum type_qualifier c_token_to_qualifier(const c_token* token);
extern enum decl_storage_class c_token_to_storage_class(const c_token* token);

extern int c_node_is_exp(tree node);
extern int c_node_is_operand(tree node);
extern int c_node_is_postfix_operator(tree node);
extern int c_list_node_is_exp(tree node, enum expr_node_kind op);

extern tree c_token_to_cst_tree(struct allocator* alloc, const c_token* token);
extern tree c_tree_exp_create(struct allocator* alloc, enum expr_node_kind op, unsigned nesting);

#endif // !C_TREE_H
