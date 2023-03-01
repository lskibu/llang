# include <stdio.h>
# include <stdlib.h>
# include <time.h>

# include "../src/util.h"
# include "../src/node.h"
# include "../src/lexer.h"
# include "../src/htable.h"
# include "../src/sym_table.h"


int main(int argc, char **argv)
{
	if(argc < 2)
		fprintf(stderr, "1 argument at least pls!\n"), exit(1);
	clock_t c=clock();
	srand(time(0));
	LLANG_LEXER *lexer = llang_lexer_create(argv[1]);
	llang_lexer_lex(lexer);
	printf("[+] Got %d tokens!\n", lexer->tokens->length);
	SYMBOL_TABLE *sym_table = __new_symbol_table(argv[1]);
	for(__LLANG_NODE *node=lexer->tokens->head;
			 node != NULL; node = node->next) {
		LL_SYMBOL *sym = __new_symbol(((LLANG_TOKEN *)node->data_ptr)->token, 0);
		int key = __symbol_table_add(sym_table, sym);
		if(key!=-1) printf("[+] Added symbol: %s with key: %d\n", sym->symbol, key);
		else printf("Already in symtable\n");
	}
	__del_symbol_table(&sym_table);
	return 0;
}
