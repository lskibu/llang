# include <stdio.h>
# include <stdlib.h>

# include "../src/util.h"
# include "../src/node.h"
# include "../src/lexer.h"

int main(int argc, char **argv)
{
	if(argc < 2)
		fprintf(stderr, "1 argument at least pls!\n"), exit(1);
	LLANG_LEXER *lexer = llang_lexer_create(argv[1]);
	llang_lexer_lex(lexer);
	return 0;
}
