#ifndef STREAM_H
#define STREAM_H

#include "scc.h"

#define SEOF -1

enum stream_kind
{
        sk_file,
        sk_string,
};

struct sstream_base
{
        const char* pos;
        const char* string;
        int eof;
};

struct fstream_base
{
        FILE* file;
};

struct stream_base
{
        enum stream_kind kind;
        const char* context;
        union
        {
                struct sstream_base s;
                struct fstream_base f;
        };
};

typedef struct stream_base stream;

extern scc_err_t fstream_init(stream* stream, const char* filename, const char* mode);
extern void sstream_init(stream* stream, const char* string);
extern void stream_close(stream* stream);

extern int  stream_eof(stream* stream);
extern int  stream_get(stream* stream);
extern void stream_read(stream* stream, void* buf, size_t count);
extern const char* stream_context(stream* stream);

#endif // !STREAM_H
