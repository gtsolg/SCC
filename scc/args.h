#ifndef ARGS_H
#define ARGS_H

#include "scc.h"

struct args
{
        const char** src;
        size_t src_size;

        const char** inc;
        size_t inc_size;

} inargs;

extern int parse_args(int argc, char** argv);

#endif // !ARGS_H
