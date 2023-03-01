#ifndef __LLANG_PARSER__
#define __LLANG_PARSER__

# include "util.h"
# include "node.h"
# include "lexer.h"
# include "sym_table.h"

# define parser_error(tree, t, msg) {\
	fprintf("[!] Syntax Error %s:%d:%d: %s", msg,\
		       	(tree)->filename, (t)->lin, (t)->pos); \
}

enum LL_T_SCOPE {
	ST_GLOBAL,
	ST_PROC,
	ST_EXPR,
	ST_METHOD
};

enum LL_T_STMT {
	ST_ASSIGN,
	ST_IF,
	ST_ELSE,
	ST_WHILE,
	ST_FOR,
	ST_REPEAT,
	ST_PROC_CALL
};

struct LL_STMT {
	enum LL_T_STMT type;
	LLANG_LIST *exprs;
	struct __LL_PARSE_SCOPE *scope;
};

struct LL_VAR {
	LLANG_TOKEN *token;
	llang_bool is_static;
	llang_bool is_const;
	llang_bool is_array;
	llang_i32 arr_size;
	struct __PARSE_SCOPE *parent;
};

struct LL_PROC {
	llang_str name;
	LLANG_LIST *args;
	LL_T_TYPE ret_type;
	llang_bool is_main;
	llang_bool is_static;
	struct __LL_PARSE_SCOPE *scope;
};

struct LL_ATTRIBUTE {
	struct LL_VAR *var;
	LL_T_TYPE access_mod;
	struct LL_CLASS *parent;
};

struct LL_CLASS_METHOD {
	llang_str name;
	LLANG_LIST *args;
	LL_T_TYPE ret_type;
	LL_T_TYPE access_mod;
	llang_bool is_virtual;
	llang_bool is_overload;
	llang_bool is_static;
	struct LL_CLASS *parent;
	struct __LL_PARSE_SCOPE *scope;
};

struct LL_CLASS {
	llang_str name;
	LLANG_LIST *parent_classes;
	LLANG_LIST *impl_ifaces;
	LL_T_TYPE access_type;
	LLANG_LIST *methods;
	LLANG_LIST *attributes;
	struct __LL_PARSE_SCOPE *parent;
};

struct LL_EXPR {
	LLANG_LIST *tokens;
	struct __LL_PARSE_SCOPE *parent;	
};

struct LL_STRUCT {
	llang_bool is_union;
	LLANG_LIST *vars;
	struct __LL_PARSE_SCOPE *parent;
};

struct __LL_PARSE_SCOPE {
	enum LL_T_SCOPE type;
	LLANG_LIST *stmts;
	LLANG_LIST *vars;
	LLANG_LIST *procs;
	LLANG_LIST *strcts;
	LLANG_LIST *ifaces;
	LLANG_LIST *classes;
	
	struct __LL_PARSE_SCOPE *parent;
	LLANG_LIST *children;
};

struct __LL_PARSE_TREE {
	char *filename;
	LLANG_LEXER *lexer;
	SYMBOL_TABLE *symbol_table;
	struct __LL_PARSE_SCOPE *gscope;
};

struct __LL_PARSER {

	LLANG_LIST *out_tree;
};

typedef struct LL_VAR LL_VAR;
typedef struct LL_EXPR LL_EXPR;
typedef struct LL_STMT LL_STMT;
typedef struct LL_STRUCT LL_STRUCT;
typedef struct LL_PROC LL_PROC;
typedef struct LL_ATTRIBUTE LL_ATTRIBUTE;
typedef struct LL_CLASS_METHOD LL_CLASS_METHOD;
typedef struct LL_CLASS LL_CLASS;
typedef struct __LL_PARSE_SCOPE LL_PARSE_SCOPE;
typedef struct __LL_PARSE_TREE LL_PARSE_TREE;
typedef struct __LL_PARSER LL_PARSER;

LL_VAR *__new_var();
void __del_var(LL_VAR **var);

LL_STMT *__new_stmt();
void __del_stmt(LL_STMT **stmt);

LL_PROC *__new_proc();
void __init_proc(LL_PROC *proc);
void __del_proc(LL_PROC **proc);

LL_STRUCT *__new_struct();
void __init_struct(LL_STRUCT *strct);
void __del_struct(LL_STRUCT **strct);

LL_ATTRIBUTE *__new_attr();
void __del_attribute(LL_ATTRIBUTE **attr);

LL_CLASS_METHOD *__new_method();
void __init_method(LL_CLASS_METHOD *meth);
void __del_method(LL_CLASS_METHOD **meth);

LL_CLASS *__new_class();
void __init_class(LL_CLASS *cls);
void __del_class(LL_CLASS **cls);

LL_EXPR *__new_expr();
void __init_expr(LL_EXPR *expr);
void __expr_add_token(LL_EXPR *expr, LLANG_TOKEN *token);
void __del_expr(LL_EXPR **expr);

LL_PARSE_SCOPE *__new_scope();
void __init_scope(LL_PARSE_SCOPE *scope);
void __destroy_scope(LL_PARSE_SCOPE **scope);

LL_PARSE_TREE *__new_tree();
void __del_tree(LL_PARSE_TREE **ptree);

LL_PARSER *llang_parser_create(LLANG_LEXER *lexer);
void llang_parser_parse(LL_PARSER *parser);
void llang_parser_destroy(LL_PARSER **parser);

#endif
