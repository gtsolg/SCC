#ifndef STR_H
#define STR_H

#include "scc.h"

extern char*  strcopy(const char* str);
extern char*  strconcat(char* dst, char* src);
extern size_t streq(const char* a, const char* b, const char* ignore);
extern const char* filename(const char* file);
extern char* format(const char* format, ...);

#endif // !STR_H
