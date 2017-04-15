#include "tree.h"
#include "tree_pass_impl.h"

static int tree_match_everything_fn(tree node, void* p)
{
        return 1;
}

static int tree_match_nonnull_fn(tree node, void* p)
{
        return (int)node;
}

static int tree_match_any_exp_fn(tree node, void* p)
{
        return node && tree_is(node, tnk_exp);
}

const struct tree_match tree_match_everything = tree_match_init(tree_match_everything_fn, NULL);
const struct tree_match tree_match_nonnull    = tree_match_init(tree_match_nonnull_fn, NULL);
const struct tree_match tree_match_exp        = tree_match_init(tree_match_any_exp_fn, NULL);

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

static inline int has_conflicts(tree_pass_prefs prefs)
{
        if (!prefs)
                return 1;

        if (prefs & PASS_PREFS_CLIPPING_MATCH && prefs & PASS_PREFS_NCLIPPING_MATCH)
                return 1;

        if (prefs & PASS_PREFS_EXP_RIGHT_TO_LEFT && prefs & PASS_PREFS_EXP_LEFT_TO_RIGHT)
                return 1;

        return 0;
}

static inline void init_pass(tree_pass_prefs prefs
        , tree_insert_dispatch_table_impl* insert_table
        , tree_foreach_init_impl_fn* init
        , tree_foreach_dispose_impl_fn* dispose
        , tree_foreach_pass_impl_fn* pass)
{
        scc_assert(!has_conflicts(prefs));

        *init = tree_foreach_noclip_init_impl;
        *dispose = tree_foreach_dispose_impl;
        *pass = tree_foreach_forward_pass_impl;
        memcpy(insert_table, &std_tree_insert_table, sizeof(tree_insert_dispatch_table_impl));

        if (prefs & PASS_PREFS_CLIPPING_MATCH)
                *init = tree_foreach_clip_init_impl;
        if (prefs & PASS_PREFS_EXP_RIGHT_TO_LEFT)
                *insert_table[tnk_exp] = tree_insert_exp_rl_impl;
}

extern void tree_foreach(struct allocator* tree_alloc, tree root
        , struct tree_pass pass
        , struct tree_match match)
{
        tree_insert_dispatch_table_impl insert_table;
        tree_foreach_init_impl_fn init;
        tree_foreach_dispose_impl_fn dispose;
        tree_foreach_pass_impl_fn passf = NULL;
        init_pass(pass.prefs, &insert_table, &init, &dispose, &passf);

        int state = PASS_SHOULD_RESTART;
        while (state == PASS_SHOULD_RESTART)
        {
                union tree_node list;
                init(tree_alloc, insert_table, match.fn, match.data, &list, root);
                state = passf(&list, pass.fn, pass.data);
                dispose(tree_alloc, &list);
        }
}
