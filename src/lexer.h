#ifndef __LLANG_LEXER__
#define __LLANG_LEXER__

# include <ctype.h>
# include "util.h"
# include "node.h"

# define llang_lexer_create(uname)		__llang_lexer_create(uname)
# define llang_lexer_destroy(lexer)		__llang_lexer_destroy(&lexer)
# define llang_lexer_lex(lexer)			__llang_lexer_lex(lexer)
# define llang_lexer_cleanup(lexer)		__llang_lexer_cleanup(lexer)


# define ll_lexer_error(desc) {\
	char *buf_ptr = buf;\
	while(isspace(*(buf_ptr+buf_pos))) buf_ptr[buf_pos--] = '\0';\
	while(isspace(*buf_ptr)) buf_ptr++,lex_pos--;\
	fprintf(stderr, "[*] Lexer Error:\n" "%s:%d:%d invalid token !\n%s\n",\
		lexer->file_name, cur_lin, lex_pos, buf_ptr);\
	for(int j=0; j < lex_pos-3; j++) fprintf(stderr, " "); fprintf(stderr, "__^\n");\
	fprintf(stderr, "%s\n" , desc);\
	exit (EXIT_FAILURE);\
};

# define ll_ttype_str(type) (type==TT_NONE ? "TT_NONE" : \
type==TT_IDENT ? "TT_IDENT" : \
type==TT_NUMBER ? "TT_NUMBER" : \
type==TT_HEXNUM ? "TT_HEXNUM" : \
type==TT_STRING ? "TT_STRING" : \
type==TT_NIL ? "TT_NIL" : \
type==TT_I16 ? "TT_I16" : \
type==TT_I32 ? "TT_I32" : \
type==TT_I64 ? "TT_I64" : \
type==TT_BOOL ? "TT_BOOL" :\
type==TT_U8 ? "TT_U8" : \
type==TT_U16 ? "TT_U16" : \
type==TT_U32 ? "TT_U32" : \
type==TT_U64 ? "TT_U64" : \
type==TT_F32 ? "TT_F32" : \
type==TT_F64 ? "TT_F64" : \
type==TT_STR ? "TT_STR" : \
type==TT_VAR ? "TT_VAR" : \
type==TT_FUNC ? "TT_FUNC" : \
type==TT_PROC ? "TT_PROC" : \
type==TT_FOR ? "TT_FOR" : \
type==TT_WHILE ? "TT_WHILE" : \
type==TT_REPEAT ? "TT_REPEAT" : \
type==TT_UNTIL ? "TT_UNTIL" : \
type==TT_ENUM ? "TT_ENUM" : \
type==TT_POINT ? "TT_POINT" : \
type==TT_LABEL ? "TT_LABEL" : \
type==TT_STRUCT ? "TT_STRUCT" : \
type==TT_UNION ? "TT_UNION" : \
type==TT_USE ? "TT_USE" : \
type==TT_CLASS ? "TT_CLASS" : \
type==TT_PUBLIC ? "TT_PUBLIC" : \
type==TT_PRIVATE ? "TT_PRIVATE" : \
type==TT_STATIC ? "TT_STATIC" : \
type==TT_PROTECTED ? "TT_PROTECTED" : \
type==TT_OVERLOAD ? "TT_OVERLOAD" : \
type==TT_INTERFACE ? "TT_INTERFACE" : \
type==TT_VIRTUAL ? "TT_VIRTUAL" : \
type==TT_SUPER ? "TT_SUPER" : \
type==TT_MAIN ? "TT_MAIN" : \
type==TT_EXTERN ? "TT_EXTERN" : \
type==TT_CONSTRUCTOR ? "TT_CONSTRUCTOR" : \
type==TT_DESTRUCTOR ? "TT_DESTRUCTOR" : \
type==TT_RETURN ? "TT_RETURN" : \
type==TT_TRUE ? "TT_TRUE" : \
type==TT_FALSE ? "TT_FALSE" : \
type==TT_IF ? "TT_IF" : \
type==TT_ELSE ? "TT_ELSE" : \
type==TT_COMMA ? "TT_COMMA" : \
type==TT_SEMICOLON ? "TT_SEMICOLON" : \
type==TT_COLON ? "TT_COLON" : \
type==TT_DCOLON ? "TT_DCOLON" : \
type==TT_QUOTE ? "TT_QUOTE" : \
type==TT_DQUOTE ? "TT_DQUOTE" : \
type==TT_LBRAKET ? "TT_LBRAKET" : \
type==TT_RBRAKET ? "TT_RBRAKET" : \
type==TT_LPARENTH ? "TT_LPARENTH" : \
type==TT_RPARENTH ? "TT_RPARENTH" : \
type==TT_LACOLAD ? "TT_LACOLAD" : \
type==TT_RACOLAD ? "TT_RACOLAD" : \
type==TT_MINUS ? "TT_MINUS" : \
type==TT_PLUS ? "TT_PLUS" : \
type==TT_MULT ? "TT_MULT" : \
type==TT_DIVID ? "TT_DIVID" : \
type==TT_GREAT ? "TT_GREAT" : type==TT_LESS ? "TT_LESS" : \
type==TT_EQUAL ? "TT_EQUAL" : \
type==TT_ASSIGN ? "TT_ASSIGN" : \
type==TT_GREAT_EQUAL ? "TT_GREAT_EQUAL" : \
type==TT_LESS_EQUAL ? "TT_LESS_EQUAL" : \
type==TT_OR ? "TT_OR" : \
type==TT_AND ? "TT_AND" : \
type==TT_NOT ? "TT_NOT" : \
type==TT_BIT_OR ? "TT_BIT_OR" : \
type==TT_BIT_AND ? "TT_BIT_AND" : \
type==TT_BIT_XOR ? "TT_BIT_XOR" : \
type==TT_BIT_NOT ? "TT_BIT_NOT" : \
type==TT_SELF ? "TT_SELF" : "UNKOWN" )

enum LL__TOKEN_TYPE {
	TT_NONE,
	// data
	TT_IDENT,
	TT_NUMBER,
	TT_HEXNUM,
	TT_STRING,
	TT_NIL,
	// data types
	TT_BOOL,
	TT_I8,
	TT_I16,
	TT_I32,
	TT_I64,
	TT_U8,
	TT_U16,
	TT_U32,
	TT_U64,
	TT_F32,
	TT_F64,
	TT_STR,
	// keywords
	TT_CONST,
	TT_VAR,
	TT_FUNC,
	TT_PROC,
	TT_FOR,
	TT_WHILE,
	TT_REPEAT,
	TT_UNTIL,
	TT_LABEL,
	TT_STRUCT,
	TT_UNION,
	TT_ENUM,
	TT_USE,
	TT_CLASS,
	TT_PUBLIC,
	TT_PRIVATE,
	TT_STATIC,
	TT_PROTECTED,
	TT_OVERLOAD,
	TT_INTERFACE,
	TT_VIRTUAL,
	TT_SUPER,
	TT_MAIN,
	TT_EXTERN,
	TT_CONSTRUCTOR,
	TT_DESTRUCTOR,
	TT_RETURN,
	TT_TRUE,
	TT_FALSE,
	TT_IF,
	TT_ELSE,
	TT_SELF,
	// other symbols
	TT_POINT,
	TT_COMMA,
	TT_SEMICOLON,
	TT_COLON,
	TT_DCOLON,
	TT_QUOTE,
	TT_DQUOTE,
	TT_LBRAKET,
	TT_RBRAKET,
	TT_LPARENTH,
	TT_RPARENTH,
	TT_LACOLAD,
	TT_RACOLAD,
	TT_MINUS,
	TT_PLUS,
	TT_MULT,
	TT_DIVID,
	TT_GREAT,
	TT_LESS,
	TT_EQUAL,
	TT_ASSIGN,
	TT_GREAT_EQUAL,
	TT_LESS_EQUAL,
	TT_OR,
	TT_AND,
	TT_NOT,
	TT_BIT_OR,
	TT_BIT_AND,
	TT_BIT_XOR,
	TT_BIT_NOT
};

struct LL__TOKEN {
	enum LL__TOKEN_TYPE type;
	llang_str token;
	llang_i32 pos;
	llang_i32 lin;
}
;

typedef struct LL__TOKEN LLANG_TOKEN;

typedef enum LL__TOKEN_TYPE LL_T_TYPE;

struct LL__LEXER {
	llang_str file_name;
	LLANG_LIST *tokens;
};

typedef struct LL__LEXER LLANG_LEXER;

LLANG_TOKEN *__token_new(llang_str token, LL_T_TYPE type);
LLANG_TOKEN *__token_next(LLANG_LEXER *lexer);

LLANG_LEXER *__llang_lexer_create(llang_str file_name);
void __llang_lexer_destroy(LLANG_LEXER **lexer);
void __llang_lexer_cleanup(LLANG_LEXER *lexer);

void __llang_lexer_lex(LLANG_LEXER *lexer);

#endif

