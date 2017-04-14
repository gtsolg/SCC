#ifndef SCC_ERR_H
#define SCC_ERR_H

#define SCC_SUCCESS 0
#define SCC_ERR     1

typedef int scc_err_t;

extern int has_errors;

// doesnt affect control flow
extern void error(const char* msg, ...);
extern void warning(const char* msg, ...);

#define internal_error(msg) \
        error("file: %s line: %u %s", __FILE__, __LINE__, msg)

#endif // !SCC_ERR_H
