#include "stream.h"
#include <string.h>

#define stream_kind(pstream) (pstream)->kind

#define fstream(pstream)         (pstream)->f
#define fstream_file(pstream)    fstream(pstream).file
#define sstream(pstream)         (pstream)->s
#define sstream_pos(pstream)     sstream(pstream).pos
#define sstream_string(pstream)  sstream(pstream).string
#define sstream_eof(pstream)     sstream(pstream).eof

extern int fstream_init(stream* stream, const char* filename, const char* mode)
{
        stream_kind(stream) = sk_file;
        fstream_file(stream) = fopen(filename, mode);
        return fstream_file(stream) != 0 ? 1 : 0;
}

extern int sstream_init(stream* stream, const char* string)
{
        stream_kind(stream) = sk_string;
        sstream_string(stream) = string;
        sstream_pos(stream) = string;
        sstream_eof(stream) = !*string;
        return 1;
}

extern void stream_close(stream* stream)
{
        if (stream_kind(stream) == sk_file)
                fclose(fstream_file(stream));
}

extern int stream_eof(stream* stream)
{
        int res = 0;
        enum stream_kind kind = stream_kind(stream);
        if (kind == sk_string)
        {
                res = sstream_eof(stream);
                sstream_eof(stream) = !*sstream_pos(stream);
        }
        else if (kind == sk_file)
                res = feof(fstream_file(stream));

        return res;
}

extern int stream_get(stream* stream)
{
        if (stream_eof(stream))
                return SEOF;

        enum stream_kind kind = stream_kind(stream);
        int c = 0;
        if (kind == sk_string)
                c = *sstream_pos(stream)++;
        else if (kind == sk_file)
                c = fgetc(fstream_file(stream));

        return c;
}

static inline size_t sstream_distance_till_eof(stream* stream, size_t range)
{
        for (size_t i = 0; i < range; i++)
                if (!*(sstream_pos(stream) + i))
                        return i;
        return range;
}

extern void stream_read(stream* stream, void* buf, size_t count)
{
        if (!count || stream_eof(stream))
                return;

        enum stream_kind kind = stream_kind(stream);
        if (kind == sk_string)
        {
                size_t range = sstream_distance_till_eof(stream, count);
                memcpy(buf, sstream_pos(stream), range);
                sstream_pos(stream) += range;
        }
        else if (kind == sk_file)
                fread(buf, 1, count, fstream_file(stream));
}