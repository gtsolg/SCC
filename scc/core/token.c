#include "token.h"

extern struct token* token_create(struct allocator* alloc, unsigned short type)
{
        struct token* token = allocate(alloc, sizeof(*token));
        if (!token)
                return NULL;
        token->type = type;
        token->lang_flags = 0;
        token->loc = null_loc;
        token->node = list_null_node;
        token->dbl = 0;
        token->integer = 0;
        return token;
}

extern struct token* token_create_iloc(struct allocator* alloc, unsigned short type, uint64_t integer, struct location loc)
{
        struct token* token = token_create(alloc, type);
        if (!token)
                return NULL;
        token->integer = integer;
        token->loc = loc;
        return token;
}

extern struct token* token_create_loc(struct allocator* alloc, unsigned short type, struct location loc)
{
        struct token* token = token_create(alloc, type);
        if (!token)
                return NULL;
        token->loc = loc;
        return token;
}

extern void token_delete(struct allocator* alloc, struct token* token)
{
        deallocate(alloc, token);
}