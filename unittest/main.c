#include "c/ccommon.h"
#include "runtime.h"
#include "c/ctest.h"

static void test_expr(const char** argv, void* p)
{
        c_test_parse_expr_raw(*argv, *(argv + 1));
}

static void test_type(const char** argv, void* p)
{
        c_test_parse_type(*argv, *(argv + 1));
}

static void test_all(const char** argv, void* p)
{
        // all tests go here
}

static struct scc_arg args[] = 
{
        scc_arg_init("-c_test_expr", 2, test_expr),
        scc_arg_init("-c_test_type", 2, test_type),
        scc_arg_init("-all",         0, test_all),
};

int main(int argc, const char** argv)
{
        if (c_setup() == SCC_ERR)
                return SCC_ERR;

        struct htab arg_map = htab_init(20);
        for (int i = 0; i < sizeof(args) / sizeof(struct scc_arg); i++)
                if (htab_insert(&arg_map, hash64_str(args[i].arg), &args[i].info) == SCC_ERR)
                        return SCC_ERR;

        return parse_args(argc, argv, &arg_map);
}