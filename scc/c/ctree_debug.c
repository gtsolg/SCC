#include "ctree_debug.h"
#include "climits.h"
#include "ccommon.h"
#include "str.h"

static inline const char* ts_kind_to_str(unsigned ts_kind)
{
//        char* s = format("");
//        if (ts_kind & tk_extern)
//                s = format("%s extern", s);
//        if (ts_kind & tk_static)
//                s = format("%s static", s);
//        if (ts_kind & tk_volatile)
//                s = format("%s volatile", s);
//        if (ts_kind & tk_const)
//                s = format("%s const", s);
//        if (strlen(s) > 0)
//                s = format("%s ", s);
//        return s;
//}
//
//static inline const char* primitive_base_to_str(struct primitive_type* ptype)
//{
//        if (ptype->is_signed)
//                switch (ptype->bit_size)
//                {
//                        case 8:  return "char";
//                        case 16: return "short";
//                        case 32: return "int";
//                        case 64: return "long long";
//                        default: return "(UNKNOWN TYPE)";
//                }
//        switch (ptype->bit_size)
//        {
//                case 8:  return "unsigned char";
//                case 16: return "unsigned short";
//                case 32: return "unsigned int";
//                case 64: return "unsigned long long";
//                default: return "(UNKNOWN TYPE)";
//        }
}

static char* type_to_str(struct var_type* type, int print_base);

static char* primitive_to_str(struct var_type* type, int print_base)
{
        //if (print_base)
        //        return format("%s%S", ts_kind_to_str(type->ts_kind), primitive_base_to_str(&type->primitive));
        //else
        //        return format("%S", type->name);
}

static char* copy_char(char c, size_t times)
{
        char* str = malloc(times + 1);
        for (size_t i = 0; i < times; i++)
                str[i] = c;
        str[times] = '\0';
        return str;
}

static char* pointer_to_str(struct var_type* type, int print_base)
{
        //if (!print_base && type->is_alias)
        //        return strcopy(type->name);

        //size_t ptr_size = 0;
        //struct var_type* it = type;
        //while (it->kind == vtk_pointer)
        //{
        //        ptr_size++;
        //        it = it->pointer.type;
        //}
        //char* ts = ts_kind_to_str(type->ts_kind);
        //char* base = type_to_str(it, 0);
        //char* asterixes = copy_char('*', ptr_size);

        //if (type->ts_kind & tk_const_const)
        //        return format("%s%s%s const", ts, base, asterixes);
        //else
        //        return format("%s%s%s", ts, base, asterixes);
}

static char* vardecl_list_to_str(struct var_node* list, char* prefix);

static char* composite_to_str(struct var_type* type, int print_base)
{
        //if (!print_base || type->is_alias)
        //{
        //        if (type->is_alias)
        //                return format("%S", type->name);
        //        else if (type->kind == vtk_struct)
        //                return format("struct %S", type->name);
        //        else
        //                return format("union %S", type->name);
        //}
        //return format("struct %S\n{\n%s}", type->name, vardecl_list_to_str(type->structt.node.head, "\t"));
}

static char* type_to_str(struct var_type* type, int print_base)
{
        //enum var_type_kind kind = type->kind;
        //if (kind == vtk_primitive)
        //        return primitive_to_str(type, print_base);
        //if (kind == vtk_pointer)
        //        return pointer_to_str(type, print_base);
        //if (kind == vtk_struct || kind == vtk_union)
        //        return composite_to_str(type, print_base);
        return NULL;        
}

extern char* c_type_to_str(struct var_type* type)
{
        return type_to_str(type, 0);
}

extern char* c_typedecl_to_str(struct var_type* type)
{
        //enum var_type_kind kind = type->kind;
        //char* base = type_to_str(type, 1);
        //if (kind == vtk_struct || kind == vtk_union)
        //{
        //        if (type->is_alias)
        //                return format("typedef struct <NAME> %S;\n", type->name);
        //        else
        //                return format("%s;\n", base);
        //}
        //else
        //        return format("typedef %s %S;\n", base, type->name);
}

static char* vardecl_to_str(struct var* var)
{
        //char* type = type_to_str(var->type, !var->type->is_alias);
        //if (var->name)
        //        return format("%s %S", type, var->name);
        //else
        //        return format("%s", type);
}

extern char* c_vardecl_to_str(struct var* var)
{
        return format("%s;\n", vardecl_to_str(var));
}

static char* vardecl_list_to_str(struct var_node* list, char* prefix)
{
        //char* s = format("");
        //while (list)
        //{
        //        s = format("%S%s%s", prefix, s, c_vardecl_to_str(list->var));
        //        list = list->node.next;
        //}
        //return s;
}

extern char* c_vardecl_list_to_str(struct var_node* list)
{
        return vardecl_list_to_str(list, "");
}

extern char* c_expr_to_str(struct expr_node* expr)
{
        //if (expr_node_is_unary(expr))
        //{
        //        char* str = NULL;
        //        if (expr->op == eok_var)
        //                str = expr->unary.var->name;
        //        else if (expr->op == eok_cast)
        //                str = c_type_to_str(expr->unary.type);
        //        else
        //                str = c_expr_to_str(expr->unary.next);
        //        return format(c_opinfo[expr->op].opstr, str);
        //}

        //char* right = c_expr_to_str(expr->binary.right);
        //char* left = c_expr_to_str(expr->binary.left);
        //char* s = format(c_opinfo[expr->op].opstr, left, right);
        //return format("(%s)", s);
}

static char* assign_stmt_to_str(struct statement* stmt)
{
        //char* l = c_expr_to_str(stmt->assign.lhs);
        //if (stmt->assign.rhs)
        //        return format("%s = %s;", l, c_expr_to_str(stmt->assign.rhs));
        //return format("%s;", l);
}

static char* expr_stmt_to_str(struct statement* stmt)
{
        //return format("%s;", c_expr_to_str(stmt->expr));
}


extern char* c_statement_to_str(struct statement* stmt)
{
        //enum statement_kind kind = stmt->kind;
        //if (kind == sk_assign)
        //        return assign_stmt_to_str(stmt);
        //if (kind == sk_expr)
        //        return expr_stmt_to_str(stmt);
        //return NULL;
}

static char* ftraits_to_str(unsigned traits)
{
        //char* s = format("");

        //if (traits & ft_cdecl)
        //        s = format("%s __cdecl", s);
        //if (traits & ft_fastcall)
        //        s = format("%s __fastcall", s);
        //if (traits & ft_stdcall)
        //        s = format("%s __stdcall", s);
        //if (traits & ft_inline)
        //        s = format("%s inline", s);
        //return s;
}

static char* code_block_to_str(struct statement* stmt, char* prefix)
{
        //char* s = format("{\n");
        //while (stmt)
        //{
        //        s = format("%s%S%s\n", s, prefix, c_statement_to_str(stmt));
        //        stmt = stmt->node.next;
        //}
        //return format("%s}\n", s);
}

extern char* c_function_to_str(struct function* func)
{
        //char* s = format("%s %s %S(", ftraits_to_str(func->traits)
        //        , c_type_to_str(func->sign->res_type), func->name);
        //if (func->sign->args)
        //{
        //        struct var_node* it = func->sign->args;
        //        while (it)
        //        {
        //                s = format("%s%s", s, vardecl_to_str(it->var));
        //                if (it->node.next)
        //                        s = format("%s, ", s);
        //                it = it->node.next;
        //        }
        //}
        //s = format("%s)", s);
        //if (!func->has_body)
        //        return format("%s;\n", s);
        //return format("%s\n%s", s, code_block_to_str(func->stmts.head, "\t"));
}
