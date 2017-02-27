#include "c/ccommon.h"
#include "c/cparser.h"
#include "c/ctest.h"
#include "args.h"
#include "file.h"

int main(int argc, char** argv)
{
        c_setup();
        //c_test_parse_expr_raw("file", "(int)(a)", NULL);
        //c_test_parse_expr_raw_f("C:\\Users\\solg computing\\Desktop\\scc_ 2\\ctest\\exp\\012t.txt",
        //                        "C:\\Users\\solg computing\\Desktop\\scc_ 2\\ctest\\exp\\012t.txt");

        if (!parse_args(argc, argv))
                return 0;
        return 0;
}