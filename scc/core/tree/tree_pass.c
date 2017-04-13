#include "tree_pass.h"
#include "tree_pass_impl.h"

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

static inline int has_conflicts(tree_pass_prefs prefs)
{
        return 0;
}

static inline void init_pass(tree_pass_prefs prefs
        , tree_insert_dispatch_table_impl* insert_table
        , tree_foreach_init_impl_fn* init
        , tree_foreach_dispose_impl_fn* dispose
        , tree_foreach_pass_impl_fn* pass)
{
        scc_assert(!has_conflicts(prefs));

        *init = tree_foreach_clip_init_impl;
        *dispose = tree_foreach_dispose_impl;
        *pass = tree_foreach_forward_pass_impl;
        memcpy(insert_table, &std_tree_insert_table, sizeof(tree_insert_dispatch_table_impl));
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
                union tree_node queue;
                init(tree_alloc, insert_table, match.fn, match.data, &queue, root);
                state = passf(&queue, pass.fn, pass.data);
                dispose(tree_alloc, &queue);
        }
}
