#ifndef TREE_PASS_IMPL_H
#define TREE_PASS_IMPL_H

#include "tree.h"

typedef int(*tree_pass_fn)(tree, void*);
typedef tree_pass_fn tree_match_fn;

scc_static_assert(tnk_size == 17, "tree_pass_impl: should update tree_insert_dispatch_table_impl");

typedef void(*tree_insert_dispatch_table_impl[tnk_size])(
        struct allocator*,
        tree,             
        tree           
        );

typedef void(*tree_foreach_init_impl_fn)(
        struct allocator*,              
        tree_insert_dispatch_table_impl,
        tree_match_fn,                  
        void*,                          
        tree,                           
        tree                            
        );

typedef void(*tree_foreach_dispose_impl_fn)(
        struct allocator*,
        tree              
        );

typedef int(*tree_foreach_pass_impl_fn)(
        tree,         
        tree_pass_fn, 
        void*         
        );

extern const tree_insert_dispatch_table_impl std_tree_insert_table;

extern void tree_insert_none_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_stmt_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_decl_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_func_decl_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_exp_lr_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_exp_rl_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_list_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_list_node_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_vec_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_sign_impl(struct allocator* tree_alloc, tree queue, tree entry);
extern void tree_insert_type_impl(struct allocator* tree_alloc, tree queue, tree entry);

struct tree_queue_node_impl
{
        struct tree_list_node node;
        tree list_entry;
};

// since we use tree allocator we need to ensure that we wont overflow allocated memory
scc_static_assert(sizeof(struct tree_queue_node_impl) <= sizeof(union tree_node)
        , "struct queue_node_impl cannot be allocated with tree allocator");

#define tree_queue_node_list_entry(ptree) ((struct tree_queue_node_impl*)ptree)->list_entry

extern tree tree_queue_node_create_impl(struct allocator* tree_alloc, tree node, tree entry);
extern void tree_queue_add(struct allocator* tree_alloc, tree queue, tree node, tree entry);

extern void tree_foreach_noclip_init_impl(struct allocator* tree_alloc,
        tree_insert_dispatch_table_impl insert,
        tree_match_fn match, void* match_data,
        tree list, tree node);

extern void tree_foreach_clip_init_impl(struct allocator* tree_alloc,
        tree_insert_dispatch_table_impl insert,
        tree_match_fn match, void* match_data,
        tree list, tree node);

extern void tree_foreach_dispose_impl(struct allocator* tree_alloc, tree list);
extern void tree_foreach_forward_pass_impl(tree list, tree_pass_fn pass, void* pass_data);

#endif // !TREE_PASS_IMPL_H

