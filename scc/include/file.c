#include "file.h"
#include "scc.h"

extern size_t file_size(const char* file)
{
        FILE* in = fopen(file, "rb");
        if (!in)
                return 0;
        fseek(in, 0, SEEK_END);
        size_t size = ftell(in);
        fclose(in);
        return size;
}

extern char* read_file(const char* file)
{  
        size_t size = file_size(file);
        if (!size)
                return NULL;
        char* content = malloc(size + 1);
        if (!content)
                return NULL;
        FILE* in = fopen(file, "rb");
        if (!in)
                return NULL;
        fread(content, 1, size, in);
        content[size] = '\0';
        fclose(in);
        return content;
}