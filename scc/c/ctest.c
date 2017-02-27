#include "ctest.h"
#include "file.h"
#include "err.h"
#include "str.h"
#include "cexpr.h"
#include "ctree_print.h"

extern void c_test_parse_expr_raw_f(const char* fin, const char* fres)
{
        char* in = read_file(fin);
        if (!in)
        {
                printf("Cannot read %s\n", fin);
                return;
        }
        char* res = fres ? read_file(fres) : in;
        if (!res)
        {
                printf("Cannot read %s\n", fres);
                return;
        }
        c_test_parse_expr_raw(filename(fin), in, res);
}

extern void c_test_parse_expr_raw(const char* filename, const char* exp, const char* res)
{
        if (!res)
                res = exp;
        printf("\ntesting %s... ", filename);
        struct c_parser parser;
        struct c_symtab* symtab = c_symtab_create();
        c_parser_init(&parser, exp);
        tree e = c_parse_expr_raw(&parser, symtab, c_parser_tokens_remains(&parser));
        char* str = c_node_to_str(e);

        if (!str)
        {
                printf("FAIL at: %s\nsyntax error:\n%s", filename, exp);
                return;
        }
        if (!streq(str, res, " \n\t\r"))
        {
                printf("FAIL at: %s\ngot:\n%s\nexpected:\n%s", filename, str, res);
                return;
        }
        printf("DONE");
}
