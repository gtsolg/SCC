#include "err.h"
#include "scc.h"

static char last_error[1024];
static char last_warning[1024];

extern void scc_error(const char* msg)
{
        size_t len = strlen(msg);
        if (len > 1000)
                sprintf(last_error, "error message is too long.\n");
        else
                sprintf(last_error, "error: %s\n", msg);
}

extern void scc_warning(const char* msg)
{
        size_t len = strlen(msg);
        if (len > 1000)
                sprintf(last_warning, "warning message is too long.\n");
        else
                sprintf(last_warning, "warning: %s\n", msg);
}

extern const char* scc_last_error()
{
        return last_error;
}

extern const char* scc_last_warning()
{
        return last_warning;
}