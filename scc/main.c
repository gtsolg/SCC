#include "c/ccommon.h"
#include "c/cparser.h"
#include "c/ctest.h"
#include "args.h"
#include "file.h"

int main(int argc, char** argv)
{
        c_setup();
        if (!parse_args(argc, argv))
                return 0;
        return 0;
}