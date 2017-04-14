#ifndef RUNTIME_H
#define RUNTIME_H

#include "parse_args.h"

struct scc_arg
{
        const char* arg;
        struct arg_info info;
};

#define scc_arg_init(arg, argc, fn) { arg, arg_info_init(argc, fn, NULL) }

extern scc_err_t scc_setup(int argc, const char** argv);
extern int scc_run();

#endif // !RUNTIME_H
