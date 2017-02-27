#ifndef CTOKEN_H
#define CTOKEN_H

#include "scc.h"
#include "list.h"

enum c_token_type
{
#include "ctoken_type.inc"
};

struct c_token
{
        struct list_node node;
        enum c_token_type type;

        union
        {
                uint64_t u64;
                uint8_t  u8;
                uint32_t u32;
                float    flt;
                double   dbl;
                char*    str;
        };
};

#define c_token_init(type, data) { list_init(), type, data }
#define c_token_init_short(type) { list_init(), type, 0 }

extern struct c_token* c_token_copy(struct c_token* token);
extern struct c_token* c_token_create(enum c_token_type type, uint64_t data);
extern void            c_token_delete(struct c_token* token);
extern void            c_token_delete_list(struct c_token* head);
extern void            c_token_print_list(struct c_token* head);

#endif // !CTOKEN_H
