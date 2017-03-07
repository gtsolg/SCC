#include "c/csymtab.h"
#include "hash.h"

extern void c_symtab_init(struct c_symtab* symtab, struct htab* globl)
{
        symtab->globl = globl;
        symtab->scope_idx = 0;

        struct htab empty = htab_init(10);
        for (size_t i = 0; i < C_MAX_BLOCK_NESTING; i++)
                symtab->scope[i] = empty;
}

extern struct c_symtab* c_symtab_create()
{
        struct htab empty = htab_init(10);

        struct c_symtab* symtab = malloc(sizeof(*symtab));
        symtab->scope_idx = 0;
        for (size_t i = 0; i < C_MAX_BLOCK_NESTING; i++)
                symtab->scope[i] = empty;
        return symtab;
}

extern void c_symtab_delete(struct c_symtab* symtab)
{
        //todo
}

extern void c_symtab_enter_scope(struct c_symtab* symtab)
{
        symtab->scope_idx++;
        // check scope
}

extern void c_symtab_exit_scope(struct c_symtab* symtab)
{
        // clear htab
        symtab->scope_idx--;
}

extern void c_symtab_add_symb(struct c_symtab* symtab, tree symb, uint64_t key)
{
        htab_insert(symtab->scope + symtab->scope_idx, key, symb);
}

extern void c_symtab_add_globl_symb(struct c_symtab* symtab, tree symb, uint64_t key)
{
        htab_insert(symtab->globl, key, symb);
}

extern tree c_symtab_get_symb(struct c_symtab* symtab, uint64_t key)
{
        tree symb = NULL;
        for (size_t i = symtab->scope_idx; i != -1; i--)
        {
                symb = htab_get(symtab->scope + i, key);
                if (symb)
                        return symb;
        }
        return htab_get(symtab->globl, key);
}
