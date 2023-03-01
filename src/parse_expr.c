#  include <string.h>

#  include "util.h"
#  include "node.h"
#  include "lexer.h"
#  include "parser.h"
#  include "sym_table.h"

// returns null on syntax error
static char * token_to_symbol(__LLANG_NODE **node)
{
	LLANG_TOKEN *token = (LLANG_TOKEN *)(*node)->data_ptr;
	if(token->type!=TT_IDENT) return NULL;
	char *symbol = (char *) llang_malloc(strlen(token->token)+1);
 	strcpy(symbol, token->token);
	int flag = 0;
	for(*node = (*node)->next; *node;) {
		LLANG_TOKEN *token = (LLANG_TOKEN *)(*node)->data_ptr;
		if(token->type==TT_POINT) {
			if(flag) return NULL;
			flag = 1;
		} else if(toke->type==IDENT) {
			flag = 0;
		} else {
			if(flag) return NULL;
			else break;
		}

		symbol = llang_realloc(symbol, strlen(symbol)+strlen(token->token)+1);
		strcat(symbol, token->token);
		*node = (*node)->next;
	}
	return symbol;
};


LL_EXPR * parse_expr(LL_PARSE_TREE *tree, LL_PARSE_SCOPE *scope, __LLANG_NODE **node) 
{
	LL_EXPR *expr = __new_expr();
	LLANG_STACK stack = llang_stack_create();
	LLANG_STACK p_stack = llang_stack_create();
	int p_cur = 0;
	for(; *node; ) {
		LLANG_TOKEN *curr = (LLANG_TOKEN *)  (*node)->data_ptr;
		LLANG_TOKEN *next = (*node)->next ? (LLANG_TOKEN *) (*node)->next->data_ptr :  NULL;

		LL_T_TYPE t_cur = curr->type;
		LL_T_TYPE t_nex = next->type;

		if(t_cur==TT_NUMBER || t_cur==TT_HEXNUM) {
			if(t_nex!=TT_PLUS&&t_nex!=TT_MINUS&&t_nex!=TT_DIVID&&
			  t_nex!=TT_MUL&&t_nex!=TT_BIT_XOR&&t_nex!=TT_BIT_AND&&
			  t_nex!=TT_BIT_OR&&t_nex!=TT_OR&&t_nex!=TT_AND&&
			  t_nex!=TT_SEMICOLON) {
				parser_error(tree, next, "invalid expression");
				goto _error;
			}
		} else if(t_cur==TT_IDENT) {
			if(t_nex!=TT_PLUS&&t_nex!=TT_MINUS&&t_nex!=TT_DIVID&&
                          t_nex!=TT_MUL&&t_nex!=TT_BIT_XOR&&t_nex!=TT_BIT_AND&&
                          t_nex!=TT_BIT_OR&&t_nex!=TT_OR&&t_nex!=TT_AND&&
                          t_nex!=TT_SEMICOLON) {
                                parser_error(tree, next, "invalid expression");
                                goto _error;
                        }
			char *symbol = token_to_symbol(node);
			if(symbol==NULL) goto _error;

		} else if() {
		
		}else {
			if(t_cur==TT_SEMICOLON) break;

		}

		llang_stack_push(stack, curr);
		
		*node = (*node)->next;
	}	

_error:
	__del_expr(&expr);
	return NULL;

};


