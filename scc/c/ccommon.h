#ifndef C_COMMON_H
#define C_COMMON_H

#include "htab.h"
#include "hash.h"
#include "tree/tree.h"
#include "reader/ctoken.h"
#include "climits.h"

enum c_family_lang_flags
{
        lf_c     = 1 << 0,
        lf_cxx   = 1 << 1,
        lf_obj_c = 1 << 2,
};

extern const struct tree_exp_info c_opinfo[];

static const char* c_reswords[] = 
{
        "", // This is used to enumerate token_type from 1

#include "creswords.inc"
};

#define C_RESWORDS_SIZE (sizeof(c_reswords) / sizeof(char*))

extern uint64_t c_reswords_hash[C_RESWORDS_SIZE];
extern struct htab c_reswords_map;
extern struct htab c_const_tree;

extern scc_err_t c_setup();

#endif // !C_COMMON_H
