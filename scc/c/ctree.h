#ifndef C_TREE_H
#define C_TREE_H

#include "ccommon.h"

extern tree c_tree_create_const(struct allocator* alloc, const ctoken* cst_token);
extern tree c_tree_exp_create(struct allocator* alloc, enum operator_kind op, unsigned nesting);

#endif // !C_TREE_H
