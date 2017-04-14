#include "error.h"
#include "scc.h"

int has_errors = 0;

extern void error(const char* msg, ...)
{
        fprintf(stderr, "Error: ");
        va_list list;
        va_start(list, msg);
        vfprintf(stderr, msg, list);
        fprintf(stderr, "\n");
        has_errors = 1;
}

extern void warning(const char* msg, ...)
{
        fprintf(stderr, "Warning: ");
        va_list list;
        va_start(msg, list);
        vfprintf(stderr, msg, list);
        fprintf(stderr, "\n");
}
