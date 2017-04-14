#include "runtime.h"

static struct scc_arg args[] = 
{
        // argument initialization goes here...
        scc_arg_init("some argument", NULL, NULL)
};

extern scc_err_t scc_setup(int argc, const char** argv)
{
        if (c_setup() == SCC_ERR)
                return SCC_ERR;

        struct htab arg_map = htab_init(20);
        for (int i = 0; i < sizeof(args) / sizeof(struct scc_arg); i++)
                if (htab_insert(&arg_map, hash64_str(args[i].arg) == SCC_ERR, args + i))
                        return SCC_ERR;

        return parse_args(argc, argv, &arg_map);
}

extern int scc_run()
{
        return 0;
}