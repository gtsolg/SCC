#ifndef C_READER_H
#define C_READER_H

#include "list.h"
#include "../climits.h"
#include "stream.h"
#include "token.h"
#include "err.h"

enum c_reader_err_type
{
        cr_cannot_open_file_err = SCC_ERR,
        cr_file_is_empty_err,
        cr_no_trailing_quote_found_err,
        cr_line_is_too_big_err,
        cr_unknown_symbol_err,
};

typedef int c_reader_loc_t;

enum file_state
{
        fs_processing = 1,
        fs_processed,
};

enum c_reader_token_type
{
        crtt_eof,
        crtt_new_line,
        crtt_whitespace,
        crtt_word,
        crtt_cst_str,
        crtt_cst_char,
        crtt_numbers,
        crtt_symbols,
        crtt_size,
};

struct def
{
        const char* symb;
        int function_like;
};

#define C_READER_BUF_SIZE 512

struct c_reader
{
        char buf[C_READER_BUF_SIZE];
        char line[C_MAX_LINE_LENGTH];
        stream src[C_MAX_INCLUDE_NESTING];

        int buf_idx;
        int line_idx;
        int src_idx;
        int eof;

        unsigned line_count;
        scc_err_t* err;

        struct list_node token_buf;
        struct allocator* token_alloc;
        const char*  current_function;
        struct htab* defs;
        struct htab* file_states;

        unsigned line_preprocessed : 1;
};

extern void c_reader_init(struct c_reader* reader
        , struct allocator* token_alloc
        , scc_err_t* err
        , const char* filename
        , struct htab* defs
        , struct htab* files);

extern void c_reader_set_current_function(struct c_reader* reader, const char* func);
extern void c_reader_include_dir(struct c_reader* reader, const char* dir);
extern void c_reader_define(struct c_reader* reader, const char* symb);
extern struct token* c_read_token(struct c_reader* reader);
extern void c_reader_loc_expose(struct c_reader* reader, c_reader_loc_t loc, char** strptr, int* len);

#endif // !C_READER_H