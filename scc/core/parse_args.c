#include "parse_args.h"

extern scc_err_t parse_args(int argc, const char** argv, struct htab* arg_map)
{
        if (argc <= 1)
        {
                error("Incorrect amount of arguments: %d", argc);
                return SCC_ERR;
        }

        int i = 1;
        while (i < argc)
        {
                struct arg_info* info = htab_get(arg_map, hash64_str(argv[i]));
                if (!info)
                {
                        error("Unknown argument: %s", argv[i]);
                        return SCC_ERR;
                }
                i++;
                int remaining = argc - i;
                if (remaining < info->argc)
                {
                        error("Too few arguments got: %d expected %d", remaining, info->argc);
                        return SCC_ERR;
                }
                info->fn(argv + i, info->data);
                i += info->argc;
        }
        return SCC_SUCCESS;
}