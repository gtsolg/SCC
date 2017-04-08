#ifndef C_SYMTAB_H
#define C_SYMTAB_H

#include "htab.h"
#include "tree/tree.h"
#include "c/climits.h"

struct c_symtab
{
        struct htab* globl;
        size_t scope_idx;
        struct htab scope[C_MAX_BLOCK_NESTING];
};

extern void c_symtab_init(struct c_symtab* symtab, struct htab* globl);

extern struct c_symtab* c_symtab_create();

extern void c_symtab_delete(struct c_symtab* symtab);
extern void c_symtab_enter_scope(struct c_symtab* symtab);
extern void c_symtab_exit_scope(struct c_symtab* symtab);
            
extern void c_symtab_add_symb(struct c_symtab* symtab, tree symb, uint64_t key);
extern void c_symtab_add_globl_symb(struct c_symtab* symtab, tree symb, uint64_t key);
extern tree c_symtab_get_symb(struct c_symtab* symtab, uint64_t key);

#endif // !C_SYMTAB_H
