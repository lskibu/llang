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
	scope->exprs = llang_list_create();
	scope->strcts = llang_list_create();
	scope->classes = llang_list_create();
	scope->ifaces = llang_list_create();
	scope->children = llang_list_create();
};
void __destroy_scope(LL_PARSE_SCOPE **scope)
{
	llang_list_destroy((*scope)->vars);
	llang_list_destroy((*scope)->procs);
	llang_list_destroy((*scope)->exprs);
	llang_list_destroy((*scope)->classes);
	llang_list_destroy((*scope)->ifaces);
	llang_list_destroy((*scope)->children);
	llang_list_destroy((*scope)->strcts);
	free(*scope);
	*scope = NULL;
};

LL_PARSE_TREE *__new_tree()
{
	return (LL_PARSER_TREE *) llang_malloc(sizeof(LL_PARSER_TREE));
};
void __del_tree(LL_PARSE_TREE **ptree)
{
	free(*ptree);
	*ptree = NULL;
};

LL_PARSER *llang_parser_create(LLANG_LEXER *lexer) 
{
	return (LL_PARSER *) llang_malloc(sizeof(LL_PARSER));
};
void __init_parser(LL_PARSER *parser)
{
	parser->out_tree = __new_tree();
	parser->out_tree->gscope = __new_scope();
	__init_scope(parser->out_tree->gscope);
};

void llang_parser_parse(LL_PARSER *parser) 
{
	LL_SCOPE *gscope = __new_scope();
	__init_scope(gscope);
	gscope->type = ST_GLOBAL;
	parse_scope(parser, gscope, parser->lexer->tokens->head);
}

static void parser_scope(LL_PARSER *parser, LL_SCOPE *scope, __LLANG_NODE *node)
{
begin_parse:
	__LLANG_NODE *first = node;


# define parsing_error(msg) 	ll_parser_error(parser->lexer, curr, msg);

# define PF_NONE 	(0)
# define PF_EXPR 	(1)
# define PF_PROC 	(2)
# define PF_STRC 	(4)
# define PF_IFAC	(8)
# define PF_CLAS	(16)
# define PF_ENUM	(32)

	llang_i32 pflag = PARSE_NONE;

	for(;;)
	{
		if(!node) break;
		
		LLANG_TOKEN *prev = node->prev != NULL ? (LLANG_TOKEN *)node->prev->data_ptr : NULL;
		LLANG_TOKEN *curr = (LLANG_TOKEN *) node->data_ptr;
		LLANG_TOKEN *next = node->next != NULL ? (LLANG_TOKEN *)node->next->data_ptr : NULL;

		switch(curr->type)
		{
			case TT_NONE: ll_parser_error(parser->lexer, curr, "Toke of NONE Type"); break;
			case TT_HEXNUM:
                	case TT_NUMBER:
				      if(pflag==PF_NONE) {ll_parser_error(parser->lexer, curr, "got number on beginning of expression");};
				      else if(pflag==PF_EXPR) {
					if(next==NULL) parsing_error("No token after a number");
				      	else if(next->type!=TT_PLUS&&next->type!=TT_MINUS&&next->type!=TT_MULT&&
						next->type!=TT_DIVID&&next->type!=TT_BIT_NOT&&next->type!=TT_BIT_AND&&
						next->type!=TT_BIT_OR&&next->type!=TT_BIT_XOR&&next->type!=TT_AND&&
						next->type!=TT_OR&&next->type!=TT_NOT&&next->type!=TT_COMMA&&
						next->type!=TT_SEMICOLON&&next->type!=TT_RBRAKET&&next->type!=TT_RPARENTH&&
						next->type!=TT_RACOLAD&&next->type!=TT_EQUAL&&next->type!=TT_GREAT&&
						next->type!=TT_LESS&&next->type!=TT_GREAT_EQUAL&&next->type!=TT_LESS_EQUAL) 
					      	ll_parser_error(parser->lexer, next, "invalid token afer number");
					
				      } else if(pflag==PF_ENUM) {
				      }	else 
					      parsing_error("Invalid Token");
				      	
				      break;
                	case TT_STRING: 
				      if(pflag==PF_NONE) parsing_error("invalid expression");
				      if(pflag==PF_EXPR) {
					if(next==NULL) parsing_error("invalid expression");
					else if(next->type!=TT_PLUS&&next->type!=TT_LBRAKET&&
						next->type!=TT_RPARENTH&&next->type!=TT_SEMICOLON&&
						next->type!=TT_AND&&next->type!=TT_OR&&next->type!=TT_NOT&&
						next->type!=TT_RACOLD&&next->type!=TT_COMMA)
						ll_parser_error(parser->lexer, next, "invalid token after string");
				      } else parsing_error("invalid token");
				      break;
                	case TT_NIL: 
				      if(pflag==PF_NONE) parsing_error("got nil in beginning of expression");
				      else if(pflag==PF_EXPR) {
					if(next==NULL) parsing_error("invalid experssion");
					else if(next->type!=TT_RPARENTH&&next->type!=TT_AND&&
						next->type!=TT_OR&&next->type!=TT_NOT&&
						next->type!=TT_SEMICOLON&&next->type!=TT_COMMA)
						ll_parser_error(parser->lexer, next, "invalid token after nil");
				      }
				      else parsing_error("nil in an unvalid scope");
				     break;
                	case TT_I8: 
                	case TT_I16:
                	case TT_I32:
                	case TT_I64:
                	case TT_U8:
                	case TT_U16:
                	case TT_U32: 
                	case TT_U64: 
                	case TT_F32: 
                	case TT_F64:
                	case TT_STR:
				    if(pflag==PF_NONE) parsing_error("invalid expression");
				    else if(pflag==PF_EXPR) { 
					if(next==NULL) parsing_error("invalid expression");
					else if(next->type!=TT_ASSIGN&&next->type!=TT_SEMICOLON)
						ll_parser_error(parser->lexer, next, "Invalid Token");
				    } else if(pflag==PF_PROC) {
					if(next==NULL) parsing_error("invalid proc/func declaration");
					else if(next->type!=TT_COMMA&&next->type!=TT_RPARENTH)
						ll_parser_error(parser->lexer, next, "In proc: invalid token after type");
				    } /*else if((pflag==PF_STRCT)||(pflag==PF_CLAS)) {
					    if(next==NULL) parsing_error("invalid token in struct");
					    else if(next->type!=TT_SEMICOLON) ll_parsing_error(parser->lexer, next, "invalid token after type");
				  */  } else parsing_error("invalid token");
				     break;
                	case TT_VAR:
				     if(pflag==PF_NONE) {
					    if(next==NULL) parsing_error("invalid expression");
					    else if(next->type!=TT_IDENT) ll_parser_error(parser->lexer, next,"invaid token after var"); 
					    pflag = PF_EXPR; 
				     }
				     else if((pflag==PF_PROC)||(pflag==PF_STRCT)) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT) ll_parser_error("invalid token");
				     } else parsing_error("invalid token");
				break; 
                	case TT_FUNC:
                	case TT_PROC: 
				if(scope->type==ST_EXPR) parsing_error("invalid token in current scope");
				else if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token after proc");
					if(next->type!=TT_IDENT&&scope->global&&next->type!=TT_MAIN) 
						ll_parser_error(parser->lexer, next, "invalid token after proc");
					pflag = PF_PROC;
				} /*else if((pflag==PF_IFAC)||(pflag==PF_CLAS)) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT&&next->type!=TT_MAIN)
						ll_parser_error(parser->lexer, next, "invalid after proc/func");
					pflag|=PF_PROC;*/
				} else parsing_error("invalid token in current scope");
				break;
			case TT_IF:
			case TT_UNTIL:
			case TT_WHILE:
			case TT_FOR: 
				if(scope->type==ST_GLOBAL) parsing_error("invalid token in the current scope");
				else if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_LPARENTH)
						ll_parser_error(parser->lexer, next, "invalid token after for");
					pflag = PF_EXPR;
				} else parsing_error("invalid token");
				break;
                	
                	case TT_REPEAT: 
				if(scope->type==ST_GLOBAL) parsing_error("invalid token in the current scope");
				else if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_LACOLAD)
						ll_parser_error(parser->lexer, next, "invalid token after repeat");
					pflag = PF_EXPR;
				} else parsing_error("invalid token");
				break;
    			
                	case TT_LABEL: 
				if(scope->type==ST_GLOBAL) parsing_error("invalid token in the current scope");
				else if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid expression");
					else if(next->type!=TT_IDENT) ll_parser_error(parser->lexer, next, "invalid token after labal");
					pflag=PF_EXPR;
				} else parsing_error("invalid token");
				break;
                	case TT_STRUCT:
                	case TT_UNION: 
				if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token");
					if(next->type!=TT_IDENT)
						ll_parser_error(parser->lexer, next, "invalid token after struct/union");
					pflag=PF_STRCT;
				} else parsing_error("invalid token");
				break;
                	case TT_ENUM: 
				if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token");
					if(next->type!=TT_IDENT) ll_parser_error(parser->lexer, next, "invalid token after enum");
					pflag=PF_ENUM;
				} else parsing_error("invalid token");
				break;
                	case TT_USE: 
				if(scope->type!=ST_GLOBAL) parsing_error("'use' must be in global scope");
				else if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid expression");
					else if(next->type!=TT_IDENT) ll_parser_error(parser->lexer, next, "invalid token after use");
					pflag=PF_EXPR;
				} else parsing_error("invalid token");
				break;
                	case TT_CLASS:
			        if(scope->type==ST_EXPR) parsing_error("invalid token in current scope");	
				else if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token");
					if(next->type!=TT_IDENT) ll_parser_error(parser->lexer, next, "invalid token after 'class'");
					pflag=PF_CLAS;
				}
				break;
			case TT_PROTECTED:
			case TT_PRIVATE:
			case TT_PUBLIC:
				if(pflag==PF_NONE) parsing_error("invalid token");
				/*else if(pflag&PF_CLAS) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_COLON)
						ll_parser_error(parser->lexer, next, "invalid token after 'public/private/protected'");
				*/
				} else parsing_error("invalid token");
				break;
               
                	case TT_STATIC: 
				if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token");
					if(next->type==TT_VAR)
						pflag=PF_EXPR;
					else if(next->type==TT_PROC)
						pflag=PF_PROC;
					else if(next->type==TT_FUNC)
						pflag=PF_FUNC;
					else
						ll_parser_error(parser->lexer, next, "invalid token after 'static'");
				} /*else if(pflag&PF_CLAS) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_PROC&&next->type!=TT_FUNC&&next->type!=TT_IDENT)
						ll_parser_error(parser->lexer, next, "invalid token after 'static'");
				*/} else parsing_error("invalid token ");
				break;
                	case TT_OVERLOAD: 
				parsing_error("invalid token in current scope");
				/*if(pflag&PF_CLAS)
				{
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_SEMICOLON)
						ll_parser_error(parser->lexer, next, "invalid token  after 'overload'");
				} else parsing_error("invalid token");
				*/break;
                	case TT_INTERFACE: 
				if(scope->type==ST_EXPR) parsing_error("invalid token in current scope");
				else if(pflag==NONE) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT)
						ll_parser_error(parser->lexer, next, "invalid token after 'iface'");
					pflag=PF_IFAC;
				} else parsing_error("invalid token");
				break;
                	case TT_VIRTUAL: 
				/*if(pflag&PF_CLAS) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_SEMICOLON)
						ll_parser_error(parser->lexer, next, "invalid token after 'virtual'");
				} else*/ parsing_error("invalid token in the current scope");
				break;
                	case TT_SUPER: 
				if(scope->type!=ST_METHOD)
				/*if(pflag&PF_ClAS) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT)
						ll_parser_error(parser->lexer, next, "invalid token after 'super'");
				} else*/ paring_error("invalid token");
				break;
                	case TT_MAIN: 
				if(pflag==PF_NONE) parser_error("invalid token");
				else if(pflag==PF_PROC) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_LPARENTH) ll_parser_error(parser->lexer, next, "invalid token");
					if(scope->type!=ST_GLOBAL) curr->type = TT_IDENT;
				} 
				break;
                	case TT_EXTERN: 
				if(scope->type!=ST_GLOBAL) parsing_error("invalid token in current scope");
				else if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_VAR||next->type!=TT_CONST)
						ll_parser_error(parser->lexer, next, "invalid token after extern");
				} else parsing_error("invalid token");
				break;
                	case TT_CONSTRUCTOR: 
				parsing_error("invalid token in current scope");
				break;
                	case TT_DESTRUCTOR: 
				parsing_error("invalid token in current scope");
				break;
                	case TT_RETURN: 
				if(scope->type!=ST_PROC||scope->type!=ST_METHOD)
					parsing_error("invalid token in current scope");
				else if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT&&next->type!=TT_LPARENTH&&next->type!=TT_PLUS&&
						next->type!=TT_MINUS&&next->type!=TT_NUMBER&&next->type!=TT_HEXNUM&&
						next->type!=TT_IDENT)
						ll_parser_error(parser->lexer, next, "invalid token after 'return'");
					pflag=PF_EXPR;
				} else parsing_error("invalid token");
				break;
			case TT_FALSE:
                	case TT_TRUE: 
				if(pflag==PF_NONE) parsing_error("invalid token");
				else if(pflag==PF_EXPR) {
					if(next==NULL) parsing_error("invalid token");
					if(next->type!=TT_AND&&next->type!=TT_RPARENTH&&
					next->type!=TT_SEMICOLON&&next->type!=TT_OR&&next->type!=TT_NOT)
						ll_parser_error(parser->lexer, next, "invalid token afte 'and'");
				} else parsing_error("invalid token");
				break;
                	case TT_ELSE: 
				if(scope->type==ST_GLOBAL) parsing_error("invalid token in current scope");
				else if(pflag==PF_NONE) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_LPARENTH&&next->type!=TT_LACOLAD)
						ll_parser_error(parser->lexer, next, "invalid token after 'else'");
				} else parsing_error("invalid token");
				break;
                	case TT_SELF: 
				if(scope->type!=ST_METHOD) parsing_error("invalid token in current scope");
				else if(pflag==PF_NONE||pflag==PF_EXPR) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_POINT) ll_parser_error(parser->lexer, next, "invalid token afte 'slef'");
				} else parsing_error("invalid token");
				break;
                	case TT_COMMA: 
				if(pflag==PF_PROC) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_VAR&&next->type!=TT_IDENT)
						ll_parser_error(parser->lexer, next, "invalid token after COMMA");
				} else if(pflag==PF_CLAS) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT) ll_parser_error(parser->lexer, next, "invalid token after COMMA");
				} else if(pflag==PF_EXPR) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_NUMBER&&next->type!=TT_HEXNUM)
						ll_parser_error(parser->lexer, next, "invalid token after COMMA");
				} else parsing_error("invalid token");
				break;
                	case TT_SEMICOLON: 
				goto end_parse;
				break;
                	case TT_COLON: 
				if(pflag==PF_EXPR||pflag==PF_PROC) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_I8&&next->type!=TT_I16&&next->type!=TT_I32&&next->type!=TT_I64&&
						next->type!=TT_U8&&next->type!=TT_U16&&next->type!=TT_U32&&next->type!=TT_U64&&
						next->type!=TT_F32&&next->type!=TT_STR&&next->type!=TT_IDENT)
						ll_parser_error(parser->lexer, next, "invalid token");
				} else parsing_error("invalid token");
				break;
                	case TT_DCOLON: 
				if(pflag==PF_EXPR) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT)
						ll_parser_error(parser->lexer, next, "invalid token after ::");
				} else parsing_error("invalid token");
				break;
                	case TT_QUOTE:
                	case TT_DQUOTE: 
				parsing_error("invalid token");	
				break;
                	case TT_RBRAKET:
			       	if(pflag==PF_EXPR) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_ASSIGN&&next->type!=TT_PLUS&&next->type!=TT_MINUS&&next->type!=TT_MULT&&
						next->type!=TT_DIVID&&next->type!=TT_AND&&next->type!=TT_OR&&next->type!=TT_BIT_OR&&
						next->type!=TT_BIT_AND&&next->type!=TT_BIT_XOR&&next->type!=TT_GREAT&&next->type!=TT_LESS&&
						next->type!=TT_GREAT_EQUAL&&next->type!=TT_LESS_EQUAL&&next->type!=TT_EQUAL&&next->type!=
						TT_SEMICOLON)
						ll_parser_error(parser->lexer, next, "invalid token after ]");
				} else parsing_error("invalid token");	
				break;
			case TT_IDENT:
				if(pflag==PF_NONE||pflag==PF_EXPR||pflag==PF_PROC) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_PLUS&&next->type!=TT_MINUS&&next->type!=TT_MULT&&
						next->type!=TT_DIVID&&next->RPARENTH&&next->type!=TT_AND&&next->type!=TT_OR&&
						next->type!=TT_NOT&&next->type!=TT_BIT_AND&&next->type!=TT_BIT_OR&&
						next->type!=TT_BIT_NOT&&next->type!=TT_BIT_XOR&&next->type!=TT_COLON&&next->type!=TT_SEMICOLON&&
						next->type!=TT_GREAT&&next->type!=TT_LESS&&next->type!=TT_GREAT_LESS&&next->type!=TT_LESS_EQUAL&&
						next->type!=TT_EQUAL)
						ll_parser_error(parser->lexer, next, "invalid token after identifier");
					pflag=PF_EXPR;
				} else if(pflag==PF_CLAS) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=RPARENTH&&next->type!=TT_COMMA&&next->type!=TT_LPARENTH)
						ll_parser_error(parser->lexer, next, "invalid token after identifier");
				} else if(pflag==PF_STRCT||pflag==PF_IFAC) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_LACOLAD)
						ll_parser_error("invalid token after identifier");
				} else parsing_error("invalid token");
				break;
                	case TT_LBRAKET: 
				if(pflag==PF_EXPR) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_NUMBER&&next->type!=TT_HEXNUMBER&&next->type!=TT_RBRAKET
						next->type!=TT_PLUS&&next->type!=TT_MINUS&&next->type!=TT_IDENT&&
						next->type!=TT_LPARENTH)
						ll_parser_error(parser->lexer, next, "invalid token after [");
				} else parsing_error("invalid token");
				break;
                	case TT_LPARENTH: 
				if(pflag==PF_EXPR) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT&&next->type!=TT_PLUS&&next->type!=TT_MINUS&&
						next->type!=TT_NUMBER&&next->type!=TT_HEXNUM&&next->type!=TT_STRING)	
						ll_parser_error(parser->lexer, next, "invalid token after (");
				} else if(pflag==PF_ClAS) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT&next->type!=TT_RPARENTH)
						ll_parser_error("invalid token in class declaration after (");
				} else if(pflag==PF_PROC) {
					if(next==NULL) parsing_error("invalid token");
					else if(next->type!=TT_IDENT&&next->type!=TT_PLUS&&next->type!=TT_MINUS&&next->type!=TT_LPARENTH&&
						next->type!=TT_STRING&&next->type!=TT_RPARENTH)
						ll_parser_error(parser->lexer, next, "invalid token after (");
				} else parsing_error("invalid token");
				break;
                	case TT_RPARENTH: break;
                	case TT_LACOLAD: break;
                	case TT_RACOLAD: break;
                	case TT_MINUS: break;
                	case TT_PLUS: break;
                	case TT_MULT: break;
                	case TT_DIVID: break;
                	case TT_GREAT: break;
                	case TT_LESS: break;
                	case TT_EQUAL: break;
                	case TT_ASSIGN: break;
                	case TT_GREAT_EQUAL: break;
                	case TT_LESS_EQUAL: break;
                	case TT_OR: break;
                	case TT_AND: break;
                	case TT_NOT: break;
                	case TT_BIT_OR: break;
                	case TT_BIT_AND: break;
                	case TT_BIT_XOR: break;
                	case TT_BIT_NOT: break;		
		}
		node = node->next;
	}
end_parse:

done_parse:

};

void llang_parser_destroy(LL_PARSER **parser)
{
	llang_lexer_destroy(&(*parser)->lexer);
	free(*parser);
	*parser = NULL;
}
