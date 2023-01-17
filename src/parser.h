#ifndef __LLANG_PARSER__
#define __LLANG_PARSER__

# include "util.h"
# include "node.h"
# include "lexer.h"

enum LL_T_EXPR {
	ET_ARETHM,
	ET_BOOL,
	ET_STR,
	ET_VOID,
	ET_OBJ_REF,
};

enum LL_T_STMT {
	ST_IF,
	ST_ELSE,
	ST_WHILE,
	ST_REPEAT_UNTIL,
	ST_SWITCH_CASE,
	ST_FUNC_CALL,
	ST_PROC_CALL,
	ST_ARETHM,
	ST_BOOL	
};

struct LL_VAR {
	llang_str name;
	llang_str type;
	llang_bool is_static;
	llang_bool is_const;
	struct __PARSE_SCOPE *parent;
};

struct LL_PROC {
	llang_str name;
	LLANG_LIST args;
	llang_str ret_type;
	llang_bool is_main;
	llang_bool is_static;
	struct __LL_PARSE_SCOPE *scope;
};

struct LL_CLASS_METHOD {
	llang_str name;
	LLANG_LIST args;
	llang_bool is_virtual;
	llang_bool is_overload;
	struct LL_CLASS *parent;
	struct __LL_PARSE_SCOPE *scope;
};

struct LL_CLASS {
	llang_str name;
	LLANG_LIST *implem;
	llang_i32 access_type;
	LLANG_LIST *methods;
	LLANG_LIST *attributes;
	LLANG_LIST *templ_t;
	struct __LL_PARSE_SCOPE *parent;
}

struct LL_EXPR {
	LL_T_EXPR ret_type;
	LLANG_LIST *tokens;

	struct LL_STMT *parent;	
};

struct LL_STMT {
	enum LL_T_STMT stmt_type;
	LLANG_LIST *expr;
	struct __LL_PARSE_SCOPE *parent;
};

struct __LL_PARSE_SCOPE {
	llang_bool global;
	LLANG_LIST *stmts;
	LLANG_LIST *vars;
	LLANG_LIST *procs;
	LLANG_LIST *ifaces;
	LLANG_LIST *classes;
	
	struct __LL_PARSE_SCOPE *parent;
	LLANG_LIST *children;
	llang_i32 nchild;
};

struct __LL_PARSE_TREE {
	struct __LL_PARSE_SCOPE *gscope;
};

struct __LL_PARSER {
	LLANG_LIST *lexers;
	struct __PARSE_TREE *out_tree;
};

typedef __LL_PARSE_TREE LL_PARSE_TREE;
typedef __LL_PARSER LL_PARSER;


LL_PARSER *llang_parser_create(LLANG_LEXER *lexer);


#endif
