#include "ctest.h"
#include "parse/cparse.h"
#include "tree/tree.h"
#include "str.h"

#define C_MAX_ANSWER_LEN 1000

struct c_test_env
{
        struct c_parser parser;
        struct c_reader reader;
        struct pool tree_pool;
        struct tree_index globl;
        char answer[C_MAX_ANSWER_LEN];
};

typedef void(*c_test_fn)(struct c_test_env*, char*);

static scc_err_t c_test_envieronment_initf(struct c_test_env* env, const char* fin, const char* fans)
{
        stream fres;
        if (fstream_init(&fres, fans, "r"))
        {
                printf("cannot open: %s", fans);
                stream_close(&fres);
                return SCC_ERR;
        }
        memset(env->answer, 0, C_MAX_ANSWER_LEN);
        stream_read(&fres, env->answer, C_MAX_ANSWER_LEN);
        stream_close(&fres);

        htab_initf(&env->globl, 0, STD_ALLOC);
        tree_index_initf(&env->globl);
        pool_initf(&env->tree_pool, sizeof(union tree_node), 10, STD_ALLOC);
        c_reader_init(&env->reader, &env->parser.c_token_alloc, &env->parser.err, fin, NULL, NULL);

        if (c_parser_err(&env->parser))
        {
                printf("cannot open: %s", fin);
                return SCC_ERR;
        }

        c_parser_init(&env->parser, &env->globl, &env->reader, &env->tree_pool);
        c_parser_init_first_token(&env->parser);
        return SCC_SUCCESS;
}

static void c_test_envieronment_dispose(struct c_test_env* env)
{
        c_parser_shutdown(&env->parser);
        // ...
}

static int c_test_passed(const char* fin, const char* res, const char* ans)
{
        if (!res)
        {
                printf("FAIL at: %s\nsyntax error:\n%s\n", fin, res);
                return 0;
        }
        else if (!streq(res, ans, " \n\t\r"))
        {
                printf("FAIL at: %s\ngot:\n%s\nexpected:\n%s\n", fin, res, ans);
                return 0;
        }
        return 1;
}


static void c_test(const char* fin, const char* fans, c_test_fn test)
{
        printf("\ntesting %s... ", filename(fin));
        struct c_test_env env;
        if (c_test_envieronment_initf(&env, fin, fans))
        {
                c_test_envieronment_dispose(&env);
                return;
        }

        c_parser_set_on_err(&env.parser)
        {
                printf("unexpected error %d", c_parser_err(&env.parser));
                c_test_envieronment_dispose(&env);
                return;
        }

        char res[1024] = { 0 };
        test(&env, res);

        if (c_test_passed(fin, res, env.answer))
                printf("DONE");

        c_test_envieronment_dispose(&env);
}

static void c_test_parse_expr_raw_fn(struct c_test_env* env, char* buf)
{
        tree exp = c_parse_expr_raw(&env->parser, c_parser_tokens_remains(&env->parser));
        c_exp_to_str(buf, exp, TREE_NULL);
}

extern void c_test_parse_expr_raw(const char* fin, const char* fans)
{
        c_test(fin, fans, c_test_parse_expr_raw_fn);
}

static void c_test_parse_type_fn(struct c_test_env* env, char* buf)
{
        tree type = c_parse_type(&env->parser, c_parser_tokens_remains(&env->parser));
        c_type_to_str(buf, type);
}

extern void c_test_parse_type(const char* fin, const char* fans)
{
        c_test(fin, fans, c_test_parse_type_fn);
}