#include "ctest.h"
#include "cparser.h"
#include "cprint.h"
#include "str.h"

extern void c_test_parse_expr_raw(const char* input, const char* result)
{
        printf("\ntesting %s... ", filename(input));
        stream fres;
        if (fstream_init(&fres, result, "r"))
        {
                printf("cannot open: %s", result);
                return;
        }
        char res[1000] = { 0 };
        stream_read(&fres, res, 1000);

        struct c_parser parser;
        struct c_reader reader;
        struct pool tree_pool;
        struct c_symtab symtab;
        struct htab globl;

        htab_initf(&globl, 0, STD_ALLOC);
        c_symtab_init(&symtab, &globl);
        pool_initf(&tree_pool, sizeof(struct tree_node), 10, STD_ALLOC);
        c_reader_init(&reader, &parser.c_token_alloc, &parser.err, input, NULL, NULL);
        if (c_parser_err(&parser))
        {
                printf("cannot open: %s", input);
                return;
        }
        c_parser_init(&parser, &reader, &tree_pool);
        c_parser_init_first_token(&parser);

        c_parser_set_on_err(&parser)
        {
                printf("unexpected error %d", c_parser_err(&parser));
                c_parser_shutdown(&parser);
                return;
        }
        tree exp = c_parse_expr_raw(&parser, &symtab, c_parser_tokens_remains(&parser));
        char* str = c_node_to_str(exp);

        if (!str)
        {
                printf("FAIL at: %s\nsyntax error:\n%s\n", input, str);
                return;
        }
        if (!streq(str, res, " \n\t\r"))
        {
                printf("FAIL at: %s\ngot:\n%s\nexpected:\n%s\n", input, str, res);
                return;
        }
        printf("DONE");
}
