#include "ctoken.h"
#include "scc.h"
#include "pool.h"
#include "ccommon.h"

struct pool token_pool = pool_init(sizeof(struct c_token), 256);

extern struct c_token* c_token_create(enum c_token_type type, uint64_t data)
{
        struct c_token* t = malloc(sizeof(*t));
        if (!t)
                return NULL;
        t->node.next = NULL;
        t->node.prev = NULL;
        t->type = type;
        t->u64 = data;
        return t;
}

extern struct c_token* c_token_copy(struct c_token* token)
{
        return c_token_create(token->type, token->u64);
}

extern void c_token_delete(struct c_token* token)
{
        free(token);
}

extern void c_token_delete_list(struct c_token* head)
{
        struct c_token* it = head->node.next;
        struct c_token* prev = head;
        while (it)
        {
                c_token_delete(prev);
                prev = it;
                it = it->node.next;
        }
        c_token_delete(prev);
}

extern void c_token_print_list(struct c_token* head)
{
        while (head)
        {
                enum c_token_type type = head->type;
                if (type == ctt_identifier || type == ctt_const_string)
                        printf("%s\n", head->str);
                else if (type == ctt_const_int)
                        printf("%llu\n", head->u64);
                else
                        printf("%s\n", c_reswords[head->type]);
                head = head->node.next;
        }
        printf("\n");
}