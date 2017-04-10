#include "tree_pass.h"

static int tree_match_everything_fn(tree node, void* p)
{
        return 1;
}

static int tree_match_nonnull_fn(tree node, void* p)
{
        return (int)node;
}

const struct tree_match tree_match_everything = tree_match_init(tree_match_everything_fn, NULL);
const struct tree_match tree_match_nonnull = tree_match_init(tree_match_nonnull_fn, NULL);

extern struct tree_pass tree_pass_initf(tree_pass_fn fn, void* data, tree_pass_prefs prefs)
{
        struct tree_pass p = tree_pass_init(fn, data, prefs);
        return p;
}

extern struct tree_match tree_match_initf(tree_match_fn fn, void* data)
{
        struct tree_match m = tree_match_init(fn, data);
        return m;
}

static int tree_match_exp_fn(tree node, void* node_kind)
{
        if (!node || !tree_is(node, tnk_exp))
                return 0;
        return tree_exp_is(node, *(enum expr_node_kind*)node_kind);
}

extern struct tree_match tree_match_exp_initf(enum expr_node_kind* kind)
{
        return tree_match_initf(tree_match_exp_fn, kind);
}

static void tree_skip(struct allocator* tree_alloc, tree list, tree node, tree_pass_prefs prefs)
{
        return;
}

static void tree_add_decl(struct allocator* tree_alloc, tree list, tree decl, tree_pass_prefs prefs)
{
        tree_list_push_back_node(list, tree_alloc, tree_decl_id(decl));
        tree_list_push_back_node(list, tree_alloc, tree_decl_base(decl));
}

static void tree_add_stmt(struct allocator* tree_alloc, tree list, tree decl, tree_pass_prefs prefs)
{
        scc_assert(0); // todo
}

static void tree_add_func_decl(struct allocator* tree_alloc, tree list, tree decl, tree_pass_prefs prefs)
{
        scc_assert(0); // todo
}

static void tree_add_exp(struct allocator* tree_alloc, tree list, tree exp, tree_pass_prefs prefs)
{
        if (prefs & PASS_PREFS_EXP_RIGHT_TO_LEFT)
        {
                tree_list_push_back_node(list, tree_alloc, tree_exp_right(exp));
                tree_list_push_back_node(list, tree_alloc, tree_exp_left(exp));
        }
        else
        {
                tree_list_push_back_node(list, tree_alloc, tree_exp_left(exp));
                tree_list_push_back_node(list, tree_alloc, tree_exp_right(exp));
        }
}

static void tree_add_list(struct allocator* tree_alloc, tree list, tree base, tree_pass_prefs prefs)
{
        struct tree_iterator it = tree_list_iterator_init(base);
        while (tree_list_iterator_valid(&it))
        {
                tree_list_push_back_node(list, tree_alloc, tree_iterator_pos(&it));
                tree_list_iterator_advance(&it);
        }
}

static void tree_add_list_node(struct allocator* tree_alloc, tree list, tree node, tree_pass_prefs prefs)
{
        tree_list_push_back_node(list, tree_alloc, tree_list_node_base(node));
}

static void tree_add_vector(struct allocator* tree_alloc, tree list, tree vec, tree_pass_prefs prefs)
{
        tree_list_push_back_node(list, tree_alloc, tree_vector_type(vec));
}

static void tree_add_sign(struct allocator* tree_alloc, tree list, tree sign, tree_pass_prefs prefs)
{
        tree_list_push_back_node(list, tree_alloc, tree_sign_restype(sign));
        tree_list_push_back_node(list, tree_alloc, tree_sign_args(sign));
}

static void tree_add_type(struct allocator* tree_alloc, tree list, tree type, tree_pass_prefs prefs)
{
        tree_list_push_back_node(list, tree_alloc, tree_type(type));
}

static void(*tree_add_siblings[])(struct allocator*, tree, tree, tree_pass_prefs) =
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

static inline void shutdown_pass(struct allocator* tree_alloc, tree list)
{
        while (!tree_list_empty(list))
        {
                tree node = tree_list_pop_back(list);
                tree_delete(tree_alloc, node);
        }
}

extern void tree_foreach(struct allocator* tree_alloc, tree root, struct tree_pass pass, struct tree_match match)
{
        struct tree_node list = tree_list_initf(&list);
        tree_list_push_back_node(&list, tree_alloc, root);

        while (!tree_list_empty(&list))
        {
                tree node = tree_list_pop_front(&list);
                tree base = tree_list_node_base(node);
                tree_delete(tree_alloc, node);

                if (!match.fn(base, match.data))
                        continue;

                tree_add_siblings[tree_kind(base)](tree_alloc, &list, base, pass.prefs);

                switch (pass.fn(base, pass.data))
                {
                        case PASS_SHOULD_EXIT:
                                shutdown_pass(tree_alloc, &list);
                                return;

                        case PASS_SHOULD_CONTINUE:
                                break;

                        case PASS_SHOULD_RESTART:
                                tree_foreach(tree_alloc, root, pass, match);
                                return;

                        default:
                                scc_assert(0); // unknown state
                                break;
                }
        }
}
