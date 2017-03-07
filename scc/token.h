#ifndef TOKEN_H
#define TOKEN_H

#include "list.h"
#include "tree.h"
#include "str_pool.h"

struct location
{
        union
        {
                const char* file;
                tree module;
        };
        unsigned off;
        unsigned line;
};

#define init_location(file, line, off) { file, line, off }

static const struct location null_loc = { NULL, 0, 0 };

struct token
{
        struct list_node node;
        struct location loc;

        unsigned short type;
        unsigned short lang_flags;

        union
        {
                uint64_t integer;
                float flt;
                long double dbl;
                strref_t ref;
        };
};

extern struct token* token_create(struct allocator* alloc, unsigned short type);
extern struct token* token_create_iloc(struct allocator* alloc, unsigned short type, uint64_t integer, struct location loc);
extern struct token* token_create_loc(struct allocator* alloc, unsigned short type, struct location loc);
extern void token_delete(struct allocator* alloc, struct token* token);

#endif // !TOKEN_H