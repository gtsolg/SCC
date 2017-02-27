#ifndef SCC_ERR_H
#define SCC_ERR_H

extern void scc_error(const char* msg);
extern void scc_warning(const char* msg);
extern const char* scc_last_error();
extern const char* scc_last_warning();

#endif // !SCC_ERR_H
