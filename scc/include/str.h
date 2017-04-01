#ifndef STR_H
#define STR_H

#include "scc.h"

extern size_t streq(const char* a, const char* b, const char* ignore);
extern const char* filename(const char* file);
extern int strprecat(char* dst, char* src);
extern int strcatn(char* dst, size_t n, ...);
extern int strprecatn(char* dst, size_t n, ...);
extern int strwrap(char* pre, char* dst, char* post);
extern char* strend(char* str);
//extern char* format(const char* format, ...);

#endif // !STR_H
