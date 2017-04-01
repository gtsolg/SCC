#ifndef C_PRINT_H
#define C_PRINT_H

#include "tree.h"

#define C_PRINT_EXP_BUF_SIZE 128

extern void c_tree_to_str(char* buf, tree node, tree prev);
extern void c_cst_to_str(char* buf, tree cst);
extern void c_exp_to_str(char* buf, tree exp, tree prev);
extern void c_type_to_str(char* buf, tree type);

#endif // !C_PRINT_H
