# include <stdio.h>
# include <string.h>
# include <ctype.h>

# include "util.h"
# include "node.h"
# include "lexer.h"

static llang_i32 buf_len = -1;
static llang_char buf[BUFSIZ];
static llang_i32 lex_pos = 0;
static llang_i32 cur_lin = 0;


static void init_lexer()
{
	buf_len = -1;
	lex_pos = 0;
	memset(buf, 0, BUFSIZ);
};

static void fill_buffer()
{
	do {
		if((buf[++buf_len] = getchar() )== '\n') cur_lin++;
	} while(buf[buf_len] != '\n' && 
		buf[buf_len] != -1 &&
		buf_len < BUFSIZ);
	//buf[buf_len] = '\0';
	if (buf_len >= BUFSIZ) {fprintf(stderr, "Warning! Line %d is Too long.\n", cur_lin);};
}
;

LLANG_TOKEN *__token_new(llang_str token, LL_T_TYPE type)
{
	LLANG_TOKEN *ll_token = (LLANG_TOKEN *) llang_malloc(sizeof(LLANG_TOKEN));
	ll_token->type = type;
	ll_token->token = token;
	return ll_token;
};

LLANG_TOKEN *__token_next(LLANG_LEXER *lexer)
{
	// update buffer
	if(lex_pos >= buf_len || buf_len >= BUFSIZ) {
		init_lexer();
		fill_buffer();
	};
	llang_i32 i = lex_pos;
	LL_T_TYPE type = TT_NONE;

	for (;;) {
		if(lex_pos >= buf_len) break;
		
		llang_char prev = lex_pos > 0 ? buf[lex_pos-1] : '\0';
		llang_char curr = buf[lex_pos++];
		llang_char next = buf[lex_pos];
		if(curr == '\0' || curr == EOF) break;
		if(isspace(curr)) {i = lex_pos; continue;}

		switch(curr) {
			case '0'...'9':
				if(type==TT_NONE)
					type = (curr == '0' && tolower(next) == 'x') ? TT_HEXNUM : TT_NUMBER;
						
				if(type==TT_NUMBER) {
					if(isalpha(next) && tolower(next) != 'e') {ll_lexer_error("Invalid number");}
					else if(ispunct(next)) goto __done;
				} else if(type==TT_HEXNUM) {
					if(!isxdigit(next)) {ll_lexer_error("Invalid Hex Character!");}
					else if(ispunct(next) || isspace(next)) goto __done;
				} else if(type==TT_IDENT) {
					if(isspace(next) || (ispunct(next) && next != '_')) goto __done;
				}
				break;
			case '_':
			case 'A'...'Z':
			case 'a'...'z':
				if(type==TT_NONE) type = TT_IDENT;
				if(type==TT_NUMBER) {
					if(tolower(curr=='e')) {if((isdigit(next)==false) && (next != '+') 
							&& (next != '-')) {ll_lexer_error("Invalid number")}; };
				} else if(type==TT_IDENT) { if(isspace(next) || (ispunct(next) && (next != '_'))) goto __done; }
				break;

			case ':':
				if(next==':') { type = TT_DCOLON; lex_pos++; }
				else type = TT_COLON;
				goto __done; 
				
			case ';':
				type = TT_SEMICOLON;
				goto __done;
				
			case ',':
				type = TT_COMMA;
				goto __done;
				
			case '/':
				type = TT_DIVID;
				goto __done;
				
			case '-':
				if(type == TT_NUMBER && tolower(prev) == 'e' && !isdigit(next)) {ll_lexer_error("invalid number");}
				else {
				type = TT_MINUS;
				goto __done;
				}
				
			case '+':
				if(type==TT_NUMBER && tolower(prev) == 'e' && !isdigit(next)) {ll_lexer_error("invalid number");}
				else {
				type = TT_PLUS;
				goto __done;
				}
		
			case '*':
				type = TT_MULT;
				goto __done;
				
			case '&':
				type = TT_BIT_AND;
				goto __done;
				
			case '|':
				type = TT_BIT_OR;
				goto __done;
				
			case '^':
				type = TT_BIT_XOR;
				goto __done;
				
			case '!':
				type = TT_BIT_NOT;
				goto __done;
				
			case '>':
				if(next == '=') {
					type = TT_GREAT_EQUAL;
					lex_pos++;
				} else type = TT_GREAT;
				goto __done;
				
			case '<':
				if(next == '=') {
					type = TT_LESS_EQUAL;
					lex_pos++;
				} else type = TT_LESS;
				goto __done;
				
			case '=':
				if(next == '=') {
					type = TT_EQUAL;
				} else type = TT_ASSIGN;
				goto __done;
				
			case '\'':
				type = TT_QUOTE;
				goto __done;	
	
			case '"':
				type = TT_DQUOTE;	
				goto __done;
			
			case '{':
				type = TT_LACOLAD;
				goto __done;
			
			case '}':
				type = TT_RACOLAD;
				goto __done;
				
			case '(':
				type = TT_LPARENTH;
				goto __done;
				
			case ')':
				type = TT_RPARENTH;
				goto __done;
			
			case '[':
				type = TT_LBRAKET;
				goto __done;
				
			case ']':
				type = TT_RBRAKET;
				goto __done;
	
			case '#':
				while(buf[lex_pos] != '\n' && buf[lex_pos++] != EOF);
				break;
			default:
				ll_lexer_error("invalid character");
		};
	}
__done:	
	if(i==lex_pos) return NULL; // empty str
	LLANG_QUEUE q = NULL;
	if(type== TT_NONE) return NULL;
	else if(type==TT_NUMBER) {
			if(lex_pos-i > 25) ll_lexer_error("invalid width, number too long");
			llang_i32 pnt = 0;
			llang_i32 eps = 0;
			for(llang_i32 j=i;j < lex_pos; j++)
			{
				if(eps>0 && tolower(buf[j]) == 'e') ll_lexer_error("invalid number, too many eeee");
				if(tolower(buf[j])=='e') eps++;
				if(pnt>0 && buf[j] == '.') ll_lexer_error("invalid number, too many points XDD");
				if(buf[j]=='.') pnt++;
			}
	}
	else if(type==TT_IDENT) { if(lex_pos-i >= 256) ll_lexer_error("invalid identifier, Too long"); }
	else if(type==TT_QUOTE || type==TT_DQUOTE) {
			q = llang_queue_create();
			llang_bool bf = false;
			while(((type == TT_QUOTE && buf[lex_pos] != '\'') || 
				(type == TT_DQUOTE && buf[lex_pos] != '"'))
				&& lex_pos < buf_len)
				{
					if(bf==true)
					{
						if(isxdigit(buf[lex_pos]))
						{	
							llang_byte b = isdigit(buf[lex_pos]) ? buf[lex_pos] - '0' : buf[lex_pos] - 'a';
							if(isdigit(buf[lex_pos+1])) {
								b <<= 4;
								b = b | (isdigit(buf[++lex_pos]) ? buf[lex_pos] - '0' : 
										tolower(buf[lex_pos]) - 'a');
							}
							llang_queue_put(q, (llang_ptr) b);
						}
						else {ll_lexer_error("invalid hex char after \\x");};
						bf = false;
					}
					else if(buf[lex_pos] == 0x5c)
					{
						switch(tolower(buf[lex_pos+1])) 
						{ 
							case '\n': lex_pos++; fill_buffer(); break;
							case 'x': lex_pos++; bf = true; break;
							case '\'': case '"':
								if((type == TT_QUOTE && buf[lex_pos+1] == '\'') || 
								(type == TT_DQUOTE && buf[lex_pos+1] == '"')) 
								llang_queue_put(q, (llang_ptr) buf[++lex_pos]);
								break;
							case 'n':
								llang_queue_put(q, (llang_ptr) '\n');
								lex_pos++;
								break;
							case 't':
								llang_queue_put(q, (llang_ptr) '\t');
								lex_pos++;
								break;
							case 'b':
								llang_queue_put(q, (llang_ptr) '\b');
								lex_pos++;
								break;
							case 'r':
								llang_queue_put(q, (llang_ptr) '\r');
								lex_pos++;
								break;
							case '\\':
								llang_queue_put(q, (llang_ptr) '\\');
								lex_pos++;
								break;
							default:
								{ll_lexer_error("invalid char after ESC");};
								break;

						};
					} else if(buf[lex_pos]=='\n') { ll_lexer_error("invalid char : new line!!"); }
					else llang_queue_put(q, (llang_ptr) buf[lex_pos]);
					lex_pos++;
			}
			if((type==TT_QUOTE&&buf[lex_pos] != '\'') || (type==TT_DQUOTE&&buf[lex_pos]!='"')) ll_lexer_error("invalid string");
			lex_pos++; // skip last quote
			type = TT_STRING;	
	}
	llang_str s = (llang_str) llang_malloc(lex_pos-i+1);
	if(type== TT_STRING) { int n = 0; while(!llang_queue_empty(q)) s[n++] = (llang_char) llang_queue_get(q);}
	else for(int j=i;j<lex_pos;j++) s[j-i]=buf[j];
	if(q) llang_queue_destroy(&q);
	s[lex_pos-i] = '\0';	
	
	if(type==TT_IDENT) {
	if(strcmp(s, "return") == 0)
		type = TT_RETURN;
	else if(strcmp(s, "while") == 0)
		type = TT_WHILE;
	else if(strcmp(s, "for") == 0)
		type = TT_FOR;
	else if(strcmp(s, "if") == 0)
		type = TT_IF;
	else if(strcmp(s, "else")==0)
		type = TT_ELSE;
	else if(strcmp(s, "repeat")==0)
		type = TT_REPEAT;
	else if(strcmp(s, "until")==0)
		type = TT_UNTIL;
	else if(strcmp(s, "const")==0)
		type = TT_CONST;
	else if(strcmp(s, "var")==0)
		type = TT_VAR;
	else if(strcmp(s, "func")==0)
		type = TT_FUNC;
	else if(strcmp(s, "proc")==0)
		type = TT_PROC;
	else if(strcmp(s, "nil")==0)
		type = TT_NIL;
	else if(strcmp(s, "main")==0)
		type = TT_MAIN;
	else if(strcmp(s, "label")==0)
		type = TT_LABEL;
	else if(strcmp(s, "use")==0)
		type = TT_USE;
	else if(strcmp(s, "overload")==0)
		type = TT_OVERLOAD;
	else if(strcmp(s, "constructor")==0)
		type = TT_CONSTRUCTOR;
	else if(strcmp(s, "destructor")==0)
		type = TT_DESTRUCTOR;
	else if(strcmp(s, "virtual")==0)
		type = TT_VIRTUAL;
	else if(strcmp(s, "super")==0)
		type = TT_SUPER;
	else if(strcmp(s, "extern")==0)
		type = TT_EXTERN;
	else if(strcmp(s, "true")==0)
		type = TT_TRUE;
	else if(strcmp(s, "false")==0)
		type = TT_FALSE;
	else if(strcmp(s, "class")==0)
		type = TT_CLASS;
	else if(strcmp(s, "iface")==0)
		type = TT_INTERFACE;
	else if(strcmp(s, "public")==0)
		type = TT_PUBLIC;
	else if(strcmp(s, "private")==0)
		type = TT_PRIVATE;
	else if(strcmp(s, "protected")==0)
		type = TT_PROTECTED;
	else if(strcmp(s, "self")==0)
		type = TT_SELF;
	else if(strcmp(s, "static")==0)
		type = TT_STATIC;
	else if(strcmp(s, "struct")==0)
		type = TT_STRUCT;
	else if(strcmp(s, "union")==0)
		type = TT_UNION;
	else if(strcmp(s, "i8")==0)
		type = TT_I8;
	else if(strcmp(s, "i16")==0)
		type = TT_I16;
	else if(strcmp(s, "i32")==0)
		type = TT_I32;
	else if(strcmp(s, "i64")==0)
		type = TT_I64;
	else if(strcmp(s, "u8")==0)
		type = TT_U8;
	else if(strcmp(s, "u16")==0)
		type = TT_U16;
	else if(strcmp(s, "u32")==0)
		type = TT_U32;
	else if(strcmp(s, "u64")==0)
		type = TT_U64;
	else if(strcmp(s, "f32")==0)
		type = TT_F32;
	else if(strcmp(s, "f64")==0)
		type = TT_F64;
	else if(strcmp(s, "bool")==0)
		type = TT_BOOL;
	else if(strcmp(s, "str")==0)
		type = TT_STR;
	else if(strcmp(s, "enum")==0)
		type = TT_ENUM;
	}

	LLANG_TOKEN *token = __token_new(s, type);
	token->lin = cur_lin;
	token->pos = i;

	return token;
};

LLANG_LEXER *__llang_lexer_create(llang_str file_name)
{
	LLANG_LEXER *lex = (LLANG_LEXER *) llang_malloc(sizeof(LLANG_LEXER));
	lex->file_name = file_name;
	lex->tokens = llang_list_create();
	return lex;
};

void __llang_lexer_destroy(LLANG_LEXER **lexer)
{
	for(__LLANG_NODE *node = (*lexer)->tokens->head;
			node != NULL;
			node = node->next )
	{
		free(((LLANG_TOKEN *) node->data_ptr)->token);
		free(node->data_ptr);				
	}
	llang_list_destroy((*lexer)->tokens);
	free(*lexer);
	*lexer = NULL;
};

void __llang_lexer_cleanup(LLANG_LEXER *lexer)
{
	lexer->file_name = NULL;
	for(__LLANG_NODE *node = lexer->tokens->head;
			node != NULL; )
	{
		free(((LLANG_TOKEN*)node)->token);
       		__LLANG_NODE *tmp = node->next;
		free(node);
		node = tmp;
	}
	llang_list_destroy(lexer->tokens);
};

void __llang_lexer_lex(LLANG_LEXER *lexer)
{
	cur_lin = 0;
	init_lexer();
	FILE *fp = NULL;
	if(lexer->file_name) {
        	fp = freopen(lexer->file_name, "r+", stdin);
		llang_assert(fp!=NULL && "Can't open file");
	}
	fill_buffer();

	do {
		LLANG_TOKEN *token = __token_next(lexer);
		if(token != NULL) {
			printf("Token: %s\tType: %s\n", token->token, ll_ttype_str(token->type));
			llang_list_append(lexer->tokens, token);
		}
		else { printf("null token\n"); }

	} while(buf[lex_pos] != EOF);
	llang_lexer_destroy(lexer);
	if(fp) fclose(fp);
};
