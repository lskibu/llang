# include <stdio.h>
# include <stdlib.h>
# include <time.h>

# include "../src/util.h"
# include "../src/node.h"
# include "../src/lexer.h"
# include "../src/htable.h"

  llang_ptr data_to_int(void *ptr) {
    LLANG_TOKEN *token=ptr;
    char *str = token->token;
    int r = rand();
    while(*str) 
      r = r^*str++;
    return (void *)r;
  }
  llang_ptr cmp_proc(llang_ptr p1, llang_ptr p2) {
    return (llang_ptr) strcmp(((LLANG_TOKEN *)p1)->token, ((LLANG_TOKEN *)p2)->token);
  }

int main(int argc, char **argv)
{
  clock_t c;
  LLANG_LEXER *lexer;
  HASH_TABLE *table;
  int col;
  int dup;
  int total;
  int i;
  if(argc < 2)
    fprintf(stderr, "1 argument at least pls!\n"), exit(1);
  c=clock();
  srand(time(0));
  lexer = llang_lexer_create(argv[1]);
  llang_lexer_lex(lexer);
  fprintf(stderr, "Lexing took %.5f sec...\n", (float)( clock() - c) / CLOCKS_PER_SEC);
  
  table = __new_hash_table(1e7, time(0) ^ rand());
  __init_hash_table(table,(llang_lambda) cmp_proc, (llang_lambda) data_to_int);
  col= dup= total = 0;
  __LLANG_NODE *node;
  for(node=lexer->tokens->head;node != NULL; node=node->next) {
    int r = __htable_insert(table, node->data_ptr);
    if(r==-1) 
      dup++;
    fprintf(stdout, "Hash for token: %s is : %d\n", ((LLANG_TOKEN *)node->data_ptr)->token, r);
  }
  for(i=0;i < table->size; i++) {
    if(table->items[i]) 
      total++, col+=table->items[i]->collisions->length;
  }
  fprintf(stderr,"Total: %d\tDup: %d\tCol: %d\tCollision rate: %.2f%%\n", total, dup, col, (float)col*100/(col+total));
  return 0;
}
