#  include <stdio.h>
#  include <stdlib.h>
#  include "util.h"
#  include "node.h"
#  include "lexer.h"
#  include "parser.h"

static void parser_error(LLANG_LEXER *l, LLANG_TOKEN *t,llang_str error)
{
	fprintf(stderr, "Parser error: %s\n", error);
	fprintf(stderr, "In File %s:%d:%d Token: %s\n",
			l->file_name, t->lin, t->pos, t->token);
	//exit(EXIT_FAILURE);
}

LL_VAR *__new_var() 
{
	return (LL_VAR *) llang_malloc(sizeof(LL_VAR));
}; 
void __del_var(LL_VAR **var)
{
	free(*var);
	*var = NULL;
};

LL_STMT *__new_stmt()
{
	LL_STMT *stmt = (LL_STMT *) llang_malloc(sizeof(LL_STMT));
	stmt->exprs = llang_list_create();
	return stmt;
};

void __del_stmt(LL_STMT **stmt)
{
	llang_list_destroy((*stmt)->exprs);
	*stmt = NULL;
};

LL_STRUCT *__new_struct()
{
	return (LL_STRUCT *) llang_malloc(sizeof(LL_STRUCT));
}

void __init_struct(LL_STRUCT *strct)
{
	strct->vars = llang_list_create();
}

void __del_struct(LL_STRUCT **strct)
{
	llang_list_destroy((*strct)->vars);
	free(*strct);
	*strct = NULL;
}

LL_PROC *__new_proc()
{
	return (LL_PROC *) llang_malloc(sizeof(LL_PROC));
};
void __init_proc(LL_PROC *proc)
{
	proc->args = llang_list_create();
};
void __del_proc(LL_PROC **proc)
{
	llang_list_destroy((*proc)->args);
	free(*proc);
	*proc = NULL;
};

LL_ATTRIBUTE *__new_attr()
{
	return (LL_ATTRIBUTE *) llang_malloc(sizeof(LL_ATTRIBUTE));
};
void __del_attribute(LL_ATTRIBUTE **attr)
{
	free(*attr);
	*attr = NULL;	
};

LL_CLASS_METHOD *__new_method() 
{
	return (LL_CLASS_METHOD *) llang_malloc(sizeof(LL_CLASS_METHOD));
};
void __init_method(LL_CLASS_METHOD *meth)
{
	meth->args = llang_list_create();	
};
void __del_method(LL_CLASS_METHOD **meth)
{
	llang_list_destroy((*meth)->args);
	free(*meth);
	*meth = NULL;
};

LL_CLASS *__new_class()
{
	return (LL_CLASS *) llang_malloc(sizeof(LL_CLASS));
};
void __init_class(LL_CLASS *cls)
{
	cls->attributes = llang_list_create();
	cls->methods  = llang_list_create();
	cls->parent_classes = llang_list_create();
	cls->impl_ifaces = llang_list_create(); 
};
void __del_class(LL_CLASS **cls)
{
	__LLANG_NODE *node;
	llang_list_destroy((*cls)->attributes);
	llang_list_destroy((*cls)->methods);
	llang_list_destroy((*cls)->parent_classes);
	llang_list_destroy((*cls)->impl_ifaces);
	free(*cls);
	*cls = NULL;
};

LL_EXPR *__new_expr() 
{
	return (LL_EXPR *) llang_malloc(sizeof(LL_EXPR));
};
void __init_expr(LL_EXPR *expr)
{
	expr->tokens = llang_list_create();	
};
void __expr_add_token(LL_EXPR *expr, LLANG_TOKEN *token)
{
	llang_list_append(expr->tokens, (llang_ptr) token);
};
void __del_expr(LL_EXPR **expr)
{
	llang_list_destroy((*expr)->tokens);
	free(*expr);
	*expr = NULL;
};

LL_PARSE_SCOPE *__new_scope()
{
	return (LL_PARSE_SCOPE *) llang_malloc(sizeof(LL_PARSE_SCOPE));
};
void __init_scope(LL_PARSE_SCOPE *scope)
{
	scope->vars = llang_list_create();
	scope->procs = llang_list_create();
	scope->stmts = llang_list_create();
	scope->strcts = llang_list_create();
	scope->classes = llang_list_create();
	scope->ifaces = llang_list_create();
	scope->children = llang_list_create();
};
void __destroy_scope(LL_PARSE_SCOPE **scope)
{
	llang_list_destroy((*scope)->vars);
	llang_list_destroy((*scope)->procs);
	llang_list_destroy((*scope)->stmts);
	llang_list_destroy((*scope)->classes);
	llang_list_destroy((*scope)->ifaces);
	llang_list_destroy((*scope)->children);
	llang_list_destroy((*scope)->strcts);
	free(*scope);
	*scope = NULL;
};

LL_PARSE_TREE *__new_tree()
{
	return (LL_PARSE_TREE *) llang_malloc(sizeof(LL_PARSE_TREE));
};

void __del_tree(LL_PARSE_TREE **ptree)
{
	free(*ptree);
	*ptree = NULL;
};

LL_PARSER *llang_parser_create(LLANG_LEXER *lexer) 
{
	LL_PARSER *parser =  (LL_PARSER *) llang_malloc(sizeof(LL_PARSER));
	parser->out_tree = llang_list_create();
	return parser;
};


void llang_parser_parse(LL_PARSER *parser) 
{
	
}

void llang_parser_destroy(LL_PARSER **parser)
{
	free(*parser);
	*parser = NULL;
}
