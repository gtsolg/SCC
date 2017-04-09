#include "tree_pass.h"

static void tree_skip(struct allocator* tree_alloc, tree list, tree node)
{
        return;
}

static void tree_add_decl(struct allocator* tree_alloc, tree list, tree decl)
{
        tree_list_push_back_node(list, tree_alloc, tree_decl_id(decl));
        tree_list_push_back_node(list, tree_alloc, tree_decl_base(decl));
}

static void tree_add_stmt(struct allocator* tree_alloc, tree list, tree decl)
{
        scc_assert(0); // todo
}

static void tree_add_func_decl(struct allocator* tree_alloc, tree list, tree decl)
{
        scc_assert(0); // todo
}

static void tree_add_exp(struct allocator* tree_alloc, tree list, tree exp)
{
        tree_list_push_back_node(list, tree_alloc, tree_exp_left(exp));
        tree_list_push_back_node(list, tree_alloc, tree_exp_right(exp));
}

static void tree_add_list(struct allocator* tree_alloc, tree list, tree base)
{
        struct tree_iterator it = tree_list_iterator_init(base);
        while (tree_list_iterator_valid(&it))
        {
                tree_list_push_back_node(list, tree_alloc, tree_iterator_pos(&it));
                tree_list_iterator_advance(&it);
        }
}

static void tree_add_list_node(struct allocator* tree_alloc, tree list, tree node)
{
        tree_list_push_back_node(list, tree_alloc, tree_list_node_base(node));
}

static void tree_add_vector(struct allocator* tree_alloc, tree list, tree vec)
{
        tree_list_push_back_node(list, tree_alloc, tree_vector_type(vec));
}

static void tree_add_sign(struct allocator* tree_alloc, tree list, tree sign)
{
        tree_list_push_back_node(list, tree_alloc, tree_sign_restype(sign));
        tree_list_push_back_node(list, tree_alloc, tree_sign_args(sign));
}

static void tree_add_type(struct allocator* tree_alloc, tree list, tree type)
{
        tree_list_push_back_node(list, tree_alloc, tree_type(type));
}

static void(*tree_add_siblings[])(struct allocator*, tree, tree) =
{
        tree_skip,          // tnk_null
        tree_add_stmt,      // tnk_stmt
        tree_skip,          // tnk_id
        tree_add_decl,      // tnk_type_decl
        tree_add_decl,      // tnk_var_decl
        tree_add_func_decl, // tnk_func_decl
        tree_add_exp,       // tnk_exp
        tree_add_list,      // tnk_list
        tree_add_list_node, // tnk_list_node
        tree_skip,          // tnk_const_int
        tree_skip,          // tnk_const_float
        tree_skip,          // tnk_const_real
        tree_skip,          // tnk_const_string
        tree_add_vector,    // tnk_vector_type
        tree_add_sign,      // tnk_sign_type
        tree_add_type,      // tnk_type
        tree_skip,          // tnk_attrib,
};

extern void tree_foreach_alloc(struct allocator* tree_alloc, tree root, tree_pass_fn pass, void* data, tree_pass_prefs prefs)
{
        struct tree_node list = tree_list_initf(&list);
        tree_list_push_back_node(&list, tree_alloc, root);
        while (!tree_list_empty(&list))
        {
                tree node = tree_list_pop_back(&list);
                tree base = tree_list_node_base(node);
                tree_delete(tree_alloc, node);
                if (base)
                        tree_add_siblings[tree_kind(base)](tree_alloc, &list, base);
                pass(base, data);
        }
}
