#include "str.h"

extern size_t streq(const char* a, const char* b, const char* ignore)
{
        const char* s[2] = { a, b };
        size_t size = strlen(ignore);

        while (1)
        {
                for (int i = 0; i < 2; i++)
                {
                        int again = 0;
                        do
                        {
                                again = 0;
                                for (size_t j = 0; j < size; j++)
                                        while (*s[i] && *s[i] == ignore[j])
                                        {
                                                s[i]++;
                                                again = 1;
                                        }
                        } while (again);

                }
                if (*s[0] != *s[1])
                        return 0;
                if (!*s[0] && !*s[1])
                        return 1;
                s[0]++;
                s[1]++;
        }
}

extern const char* filename(const char* file)
{
        const char* it = file;
        while(*it)
                it++;
        while (*it != '\\' && *it != '/')
                it--;
        return ++it;
}

extern int strprecat(char* dst, char* src)
{
        size_t size = strlen(src);
        memmove(dst + size, dst, strlen(dst) + 1);
        memcpy(dst, src, size);
        return 0;
}

extern int strcatn(char* dst, size_t n, ...)
{
        va_list strings;
        va_start(strings, n);
        for (size_t i = 0; i < n; i++)
                strcat(dst, va_arg(strings, char*));
        return 0;
}

extern int strprecatn(char* dst, size_t n, ...)
{
        va_list strings;
        va_start(strings, n);
        for (size_t i = 0; i < n; i++)
                strprecat(dst, va_arg(strings, char*));
        return 0;
}

extern int strwrap(char* pre, char* dst, char* post)
{
        strcat(dst, post);
        strprecat(dst, pre);
        return 0;
}

extern char* strend(char* str)
{
        return str + strlen(str);
}
