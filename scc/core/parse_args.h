#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "htab.h"
#include "hash.h"

typedef void(*parse_arg_fn)(const char**, void*);

struct arg_info
{
        int argc;
        parse_arg_fn fn;
        void* data;
};

#define arg_info_init(argc, fn, data) { argc, fn, data }

extern scc_err_t parse_args(int argc, const char** argv, struct htab* arg_map);

#endif // !PARSE_ARGS_H
