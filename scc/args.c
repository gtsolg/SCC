#include "args.h"
#include "scc.h"
#include "htab.h"
#include "hash.h"
#include "c/ctest.h"

static int c_argument(int idx, int argc, char** argv)
{
        return 0;
}

static int include_argument(int idx, int argc, char** argv)
{
        return 0;
}

static int define_argument(int idx, int argc, char** argv)
{
        return 0;
}

static int test_expr_argument(int idx, int argc, char** argv)
{
        if (argc - idx < 2)
        {
                printf("incorrect amount of arguments.\n");
                return 0;
        }
        c_test_parse_expr_raw_f(argv[idx], argv[idx + 1]);
        return 2;
}

static struct htab arg_map = htab_init(30);

static void setup_arg_map()
{
#define add_arg(arg, foo) htab_insert(&arg_map, hash64_str(arg), foo)

        add_arg("-c_test_expr", test_expr_argument);
}

extern int parse_args(int argc, char** argv)
{
        if (argc <= 1)
                return 0;

        setup_arg_map();
        int i = 1;
        while (i < argc)
        {
                int(*parse_arg)(int, int, char**) = htab_get(&arg_map, hash64_str(argv[i]));
                if (!parse_arg)
                        return 0; // unknown argument
                int offset = parse_arg(++i, argc, argv);
                if (!offset)
                        return 0; // error while parsing
                i += offset;
        }
        return 1;
}
