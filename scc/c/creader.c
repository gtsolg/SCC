#include "creader.h"
#include "str.h"
#include "str_pool.h"

#define c_reader_cur_stream(preader) ((preader)->src + (preader)->src_idx)
#define c_reader_cur_lpos(preader)   ((preader)->line + (preader)->line_idx)
#define c_reader_cur_bpos(preader)   ((preader)->buf + (preader)->buf_idx)

static inline reader_reset_buf(struct c_reader* reader)
{
        memset(reader->buf, 0, C_READER_BUF_SIZE);
        reader->buf_idx = 0;
}

static inline reader_reset_line(struct c_reader* reader)
{
        memset(reader->line, 0, C_MAX_LINE_LENGTH);
        reader->line_idx = 0;
        reader->line_preprocessed = 0;
}

static inline void read_buf(struct c_reader* reader)
{
        reader_reset_buf(reader);
        stream_read(c_reader_cur_stream(reader), reader->buf, C_READER_BUF_SIZE);
}

static inline int buf_empty(struct c_reader* reader)
{
        return !reader->buf[0];
}

static inline int buf_end(struct c_reader* reader)
{
        return reader->buf_idx == C_READER_BUF_SIZE;
}

static inline int line_empty(struct c_reader* reader)
{
        return !reader->line[0];
}

static inline int line_end(struct c_reader* reader, int* line_ended)
{
        int i = 0;
        int off = reader->buf_idx;
        for (; i + off < C_READER_BUF_SIZE; i++)
        {
                int c = reader->buf[i + off];
                if (c == '\0' || c == '\n')
                {
                        i++; // skip \n
                        *line_ended = 1;
                        break;
                }
        }
        reader->buf_idx = i + off;
        return i;
}

static inline void read_line(struct c_reader* reader)
{
        reader_reset_line(reader);
        int ended = 0;
        int idx = 0;
        while (1)
        {
                if (buf_empty(reader) || buf_end(reader))
                        read_buf(reader);

                const char* start = c_reader_cur_bpos(reader);
                int off = line_end(reader, &ended);
                if (idx + off - 1 == C_MAX_LINE_LENGTH)
                {
                        *reader->err = cr_line_is_too_big_err;
                        return;
                }
                memcpy(reader->line + idx, start, off);
                idx += off;
                if (ended)
                        return;
        }
}

static void reader_init(struct c_reader* reader, struct allocator* token_alloc, scc_err_t* err
        , struct htab* defs, struct htab* files)
{
        reader->token_alloc = token_alloc;
        reader->err = err;
        reader->defs = defs;
        reader->files = files;
        *reader->err = SCC_SUCCESS;

        reader->func = NULL;
        reader->buf_idx = 0;
        reader->src_idx = 0;
        reader->line_idx = 1;
        reader->linec = 0;
        list_initf(&reader->token_buf);
        reader_reset_buf(reader);
}

static void c_reader_switch_file(struct c_reader* reader, const char* filename)
{
        if (fstream_init(c_reader_cur_stream(reader), filename, "r"))
        {
                *reader->err = cr_cannot_open_file_err;
                return;
        }
        read_line(reader);
        if (line_empty(reader))
        {
                *reader->err = cr_file_is_empty_err;
                return;
        }
        reader->src_idx++;
}

extern void c_reader_init(struct c_reader* reader, struct allocator* token_alloc, scc_err_t* err
        , const char* filename, struct htab* defs, struct htab* files)
{
        reader_init(reader, token_alloc, err, defs, files);
        c_reader_switch_file(reader, filename);
}


extern void c_reader_set_func(struct c_reader* reader, const char* func)
{
        reader->func = func;
}

extern void c_reader_include_dir(struct c_reader* reader, const char* dir)
{

}

extern void c_reader_define(struct c_reader* reader, const char* symb)
{

}

static void preprocess_line(struct c_reader* reader)
{
        reader->line_preprocessed = 1;
        // todo: del comments, merge lines ...
}

static inline int alpha(int c)
{
        return c >= 'a' && c <= 'z'
                || c >= 'A' && c <= 'Z'
                || c == '_';
}

static inline int digit(int c)
{
        return c >= '0' && c <= '9';
}

static inline int ident(int c)
{
        return alpha(c) || digit(c);
}

static inline int symbol(int c)
{
        return c >= 33 && c <= 47
                || c >= 58 && c <= 64
                || c >= 91 && c <= 94
                || c == 96
                || c >= 123 && c <= 126;
}

static inline int newline(int c)
{
        return c == '\r' || c == '\n';
}

static inline int whitespace(int c)
{
        return c == ' ' || c == '\t';
}

static inline int sequence_len(const char* string, int(*match)(int))
{
        int len = 0;
        while (match(*string++))
                len++;
        return len;
}

static inline int skip_till(const char* string, int c)
{
        int len = 0;
        while (*string++ != c)
        {
                if (!*string || len == C_MAX_LINE_LENGTH)
                        return 0;
                len++;
        }
        return len;
}

static inline struct location current_loc(struct c_reader* reader)
{
        struct location loc = init_location(NULL, reader->linec, reader->line_idx);
        return loc;
}

static struct token* read_directive(struct c_reader* reader)
{
        return NULL;
}

static struct token* read_newline(struct c_reader* reader)
{
        read_line(reader);
        return token_create(reader->token_alloc, crtt_new_line);
}

static inline c_reader_loc_t reader_loc(int idx, int len)
{
        return idx | (len << 16);
}

static inline struct token* read_string(struct c_reader* reader, enum c_reader_token_type type, int quote)
{
        struct location loc = current_loc(reader);
        reader->line_idx++;
        const char* begin = c_reader_cur_lpos(reader);
        int len = skip_till(begin, quote);
        if (!len || len >= C_MAX_LINE_LENGTH)
        {
                *reader->err = cr_no_trailing_quote_found_err;
                return NULL;
        }

        c_reader_loc_t rloc = reader_loc(reader->line_idx, len);
        reader->line_idx += len + 1;
        return token_create_iloc(reader->token_alloc, type, rloc, loc);
}

static struct token* read_cst_string(struct c_reader* reader)
{
        return read_string(reader, crtt_cst_str, '\"');
}

static struct token* read_cst_char(struct c_reader* reader)
{
        return read_string(reader, crtt_cst_char, '\'');
}

static inline struct token* read_sequence(struct c_reader* reader, enum c_reader_token_type type, int(*match)(int))
{
        struct location loc = current_loc(reader);
        const char* begin = c_reader_cur_lpos(reader);
        int len = sequence_len(begin, match);
        c_reader_loc_t rloc = reader_loc(reader->line_idx, len);
        reader->line_idx += len;
        return token_create_iloc(reader->token_alloc, type, rloc, loc);
}

static struct token* eof_token(struct c_reader* reader)
{
        return token_create(reader->token_alloc, crtt_eof);
}

extern struct token* c_read_token(struct c_reader* reader)
{
        if (!list_empty(&reader->token_buf))
                return list_pop_back(&reader->token_buf);

        if (!reader->line_preprocessed)
                preprocess_line(reader);

        int c = *c_reader_cur_lpos(reader);
        if (!c)
                return NULL; // eof

        if (c == '#')
                read_directive(reader);
        else if (newline(c))
                return read_newline(reader);
        else if (whitespace(c))
                return read_sequence(reader, crtt_whitespace, whitespace);
        else if (c == '\"')
                return read_cst_string(reader);
        else if (c == '\'')
                return read_cst_char(reader);
        else if (alpha(c))
                return read_sequence(reader, crtt_word, ident);
        else if (digit(c))
                return read_sequence(reader, crtt_numbers, digit);
        else if (symbol(c))
                return read_sequence(reader, crtt_symbols, symbol);

        *reader->err = cr_unknown_symbol_err;
        return NULL;
}

extern void c_reader_loc_expose(struct c_reader* reader, c_reader_loc_t loc, char** strptr, int* len)
{
        short* parts = (short*)&loc;
        *strptr = reader->line + parts[0];
        *len = parts[1];
}