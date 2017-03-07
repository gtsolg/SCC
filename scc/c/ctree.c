#include "ctree.h"
#include "ccommon.h"

extern tree c_tree_create_const(struct allocator* alloc, const ctoken* cst)
{
        switch (cst->type)
        {
                case ctt_const_char:
                case ctt_const_int:
                        return tree_const_int_create(alloc, cst->integer);

                case ctt_const_float:
                        return tree_const_float_create(alloc, cst->flt);

                case ctt_const_string:
                        return tree_const_string_create(alloc,
                                string_pool_get_string_by_ref(STD_STR_POOL, cst->ref));

                default:
                        return NULL;
        }
}

extern tree c_tree_exp_create(struct allocator* alloc, enum operator_kind op, unsigned nesting)
{
        return tree_exp_create(alloc, op, nesting, c_opinfo);
}