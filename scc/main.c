#include "runtime.h"

int main(int argc, char** argv)
{
        return scc_setup(argc, argv) == SCC_ERR ? -1 : scc_run();
}