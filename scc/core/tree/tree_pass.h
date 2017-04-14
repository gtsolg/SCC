#ifndef TREE_PASS_H
#define TREE_PASS_H

#include "tree.h"

typedef int(*tree_pass_fn)(tree node, void*);

#define PASS_SHOULD_EXIT     0
#define PASS_SHOULD_CONTINUE 1
#define PASS_SHOULD_RESTART  2

typedef uint64_t tree_pass_prefs;

#define PASS_PREFS_EXP_RIGHT_TO_LEFT (1 << 0)
#define PASS_PREFS_EXP_LEFT_TO_RIGHT (1 << 1)

// if node doesnt match its siblings will not be added
#define PASS_PREFS_CLIPPING_MATCH    (1 << 2)
#define PASS_PREFS_NCLIPPING_MATCH   (1 << 3)

#define PASS_PREFS_NONE (PASS_PREFS_EXP_LEFT_TO_RIGHT | PASS_PREFS_NCLIPPING_MATCH)

struct tree_pass
{
        tree_pass_fn fn;
        void* data;
        tree_pass_prefs prefs;
};

#define tree_pass_init(fn, data, prefs) { fn, data, prefs }

// return 0 if node doesnt match, nonnull in the other case
typedef tree_pass_fn tree_match_fn;

struct tree_match
{
        tree_match_fn fn;
        void* data;
};

#define tree_match_init(fn, data) { fn , data }

extern struct tree_pass tree_pass_initf(tree_pass_fn fn, void* data, tree_pass_prefs prefs);

extern const struct tree_match tree_match_everything;
extern const struct tree_match tree_match_nonnull;

extern struct tree_match tree_match_initf(tree_match_fn fn, void* data);
extern struct tree_match tree_match_exp_initf(enum expr_node_kind* kind);

// allocates its own stack of nodes
// passes expressions left-to-right by default e.g:
//       4      
//     /   \    
//    3     6   
//   / \   / \  
//  1   2 5   7 
extern void tree_foreach(struct allocator* tree_alloc, tree root, struct tree_pass pass, struct tree_match match);

#endif // !TREE_PASS_H
