#ifndef C_PRINT_H
#define C_PRINT_H

#include "tree.h"

extern char* c_const_to_str(tree cst);
extern char* c_node_to_str(tree node);
extern char* c_exp_to_str(tree exp);
extern char* c_type_to_str(tree type);

#endif // !C_PRINT_H
