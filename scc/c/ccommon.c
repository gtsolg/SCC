#include "ccommon.h"

uint64_t c_reswords_hash[C_RESWORDS_SIZE] = { 0 };

struct htab c_reswords_map = htab_init(400);
struct htab c_const_tree = htab_init(1000);

static scc_err_t setup_reswords()
{
        for (size_t i = 0; i < C_RESWORDS_SIZE; i++)
        {
                c_reswords_hash[i] = hash64_str(c_reswords[i]);
                if (htab_insert(&c_reswords_map, c_reswords_hash[i], (void*)i))
                        return SCC_ERR;
        }
        return SCC_SUCCESS;
}

static scc_err_t add_deftype(enum type_kind kind, int tokenc, ...)
{
        va_list list;
        va_start(list, tokenc);

        uint64_t h = 0;
        for (size_t i = 0; i < tokenc; i++)
                h += c_reswords_hash[va_arg(list, int)];

        tree node = tree_type_create(STD_ALLOC, kind, tq_unqualified, NULL);
        return node ? htab_insert(&c_const_tree, h, node) : SCC_ERR;
}

static scc_err_t setup_deftypes()
{
        return 
                   add_deftype(tk_void,   1, ctt_void)
                || add_deftype(tk_float,  1, ctt_float)
                || add_deftype(tk_double, 1, ctt_double)
                || add_deftype(tk_double, 2, ctt_long, ctt_double)
             
                || add_deftype(tk_int8,  1, ctt_char)
                || add_deftype(tk_int8,  2, ctt_signed,   ctt_char)
                || add_deftype(tk_uint8, 2, ctt_unsigned, ctt_char)
               
                || add_deftype(tk_int16,  1, ctt_short)
                || add_deftype(tk_int16,  2, ctt_short,    ctt_int)
                || add_deftype(tk_int16,  2, ctt_signed,   ctt_short)
                || add_deftype(tk_int16,  3, ctt_signed,   ctt_short, ctt_int)
                || add_deftype(tk_uint16, 2, ctt_unsigned, ctt_short)
                || add_deftype(tk_uint16, 3, ctt_unsigned, ctt_short, ctt_int)
           
                || add_deftype(tk_int32,  1, ctt_int)
                || add_deftype(tk_int32,  1, ctt_signed)
                || add_deftype(tk_int32,  2, ctt_signed,   ctt_int)
                || add_deftype(tk_uint32, 1, ctt_unsigned)
                || add_deftype(tk_uint32, 2, ctt_unsigned, ctt_int)
    
                || add_deftype(tk_int32,  1, ctt_long)
                || add_deftype(tk_int32,  2, ctt_long,     ctt_int)
                || add_deftype(tk_int32,  2, ctt_signed,   ctt_long)
                || add_deftype(tk_int32,  3, ctt_signed,   ctt_long, ctt_int)
                || add_deftype(tk_uint32, 2, ctt_unsigned, ctt_long)
                || add_deftype(tk_uint32, 3, ctt_unsigned, ctt_long, ctt_int)

                || add_deftype(tk_int64,  2, ctt_long,     ctt_long)
                || add_deftype(tk_int64,  3, ctt_long,     ctt_long, ctt_int)
                || add_deftype(tk_int64,  3, ctt_signed,   ctt_long, ctt_long)
                || add_deftype(tk_int64,  4, ctt_signed,   ctt_long, ctt_long, ctt_int)
                || add_deftype(tk_uint64, 3, ctt_unsigned, ctt_long, ctt_long)
                || add_deftype(tk_uint64, 4, ctt_unsigned, ctt_long, ctt_long, ctt_int)
                ? SCC_ERR : SCC_SUCCESS;
}

extern scc_err_t c_setup()
{
        return setup_reswords()
            || setup_deftypes()
                ? SCC_ERR : SCC_SUCCESS;
}
