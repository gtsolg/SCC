#include "scc.h"
#include "c/ccommon.h"
#include "args.h"

int main(int argc, char** argv)
{
        if (c_setup())
                return SCC_ERR;
        parse_args(argc, argv);
        return 0;
}