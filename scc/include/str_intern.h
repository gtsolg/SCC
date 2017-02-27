#ifndef STR_INTERN_H
#define STR_INTERN_H

#include "scc.h"

extern const char* str_intern(const char* str);
extern const char* str_intern_from(const char* pos, size_t len);
extern int str_interned(const char* str);

#endif // !STR_INTERN_H
