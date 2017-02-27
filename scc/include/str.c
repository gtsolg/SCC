#include "str.h"

extern char* strcopy(const char* str)
{
        size_t len = strlen(str);
        char* copy = malloc(len + 1);
        return strcpy(copy, str);
}

extern char* strconcat(char* dst, char* src)
{
        if (!src)
                return dst;

        size_t dsize = dst ? strlen(dst) : 0;
        size_t ssize = strlen(src);

        if (!dst)
        {
                dst = malloc(dsize);
                strcpy(dst, src);
                return dst;
        }
        char* n = malloc(dsize + ssize + 1);
        strcpy(n, dst);
        strcat(n + dsize, src);
        return n;
}

extern size_t streq(const char* a, const char* b, const char* ignore)
{
        char* s[2] = { a, b };
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

struct rep
{
        const char* format;
        char* (*before)(void*);
        size_t(*len)(void*);
        void(*after)(void*);
};

static char* def_before(void* ptr)
{
        return ptr;
}

static void def_after(void* ptr)
{
        return;
}

static void free_after(void* ptr)
{
        free(ptr);
}

static struct rep def_rtable[] =
{
        { "%s",  def_before, strlen, free_after },
        { "%S",  def_before, strlen, def_after  },
};

static int contains(const char* a, const char* b)
{
        while (*a == *b)
        {
                a++;
                b++;

                if (!*b)
                        return 1;
        }
        return 0;
}

static char* vformat(const char* format, struct rep* rtable, size_t rsize, va_list list)
{
        va_list strings;
        va_copy(strings, list);

        size_t size = 0;
        for (const char* it = format; *it; it++)
        {
                int found = 0;
                for (size_t i = 0; i < rsize; i++)
                        if (contains(it, rtable[i].format))
                        {
                                found = 1;
                                size += rtable[i].len(va_arg(list, void*));
                                break;
                        }
                if (!found)
                        size++;
        }
        char* res = malloc(size + 1);
        char* rit = res;
        const char* it = format;
        for (; *it; it++, rit++)
        {
                int found = 0;
                for (size_t i = 0; i < rsize; i++)
                        if (contains(it, rtable[i].format))
                        {
                                found = 1;
                                void* arg = va_arg(strings, void*);
                                char* str = rtable[i].before(arg);
                                size_t len = strlen(str);
                                memcpy(rit, str, len);
                                rtable[i].after(arg);
                                rit += len - 1;
                                it++;
                                break;
                        }
                if (!found)
                        *rit = *it;
        }
        *rit = '\0';
        return res;

}

extern char* format(const char* format, ...)
{
        va_list list;
        va_start(list, format);
        return vformat(format, def_rtable, 2, list);
}