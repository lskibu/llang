#  include "util.h"
#  include "node.h"
#  include "htable.h"
#  include "sym_table.h"

#  include <time.h>
#  include <string.h>

# define SYM_TABLE_SIZE (1024*1024*4)

static int symbol_to_int32(LL_SYMBOL *symbol)
{
  int result = 0;
  char *str = symbol->symbol;
  int k = 0;
  while(*str) {
    result = result ^ (*str++ << k++*8);
    if(k>4) k = 0;
  }
  return result;
};

static int symbol_cmp_proc(llang_ptr ptr1, llang_ptr ptr2)
{
  return strcmp(((LL_SYMBOL *)ptr1)->symbol, 
		((LL_SYMBOL *)ptr2)->symbol);
};

LL_SYMBOL *__new_symbol(char *symbol,LL_T_SYM type)
{
  LL_SYMBOL *sym = (LL_SYMBOL *) llang_malloc(sizeof(LL_SYMBOL));
  sym->symbol = symbol;
  sym->type = type;
  return sym;
};

void __del_symbol(LL_SYMBOL **symbol)
{
  free(*symbol);
  *symbol = NULL;
};

SYMBOL_TABLE *__new_symbol_table(char *unit)
{
  SYMBOL_TABLE *sym_table = (SYMBOL_TABLE *) llang_malloc(sizeof(SYMBOL_TABLE));
  sym_table->unit = unit;
  sym_table->table = __new_hash_table(SYM_TABLE_SIZE, time(0) ^ SYM_TABLE_SIZE);
  __init_hash_table(sym_table->table, symbol_cmp_proc, symbol_to_int32);
  return sym_table;
};

void __del_symbol_table(SYMBOL_TABLE **sym_table)
{
  __htable_destroy(&(*sym_table)->table);
  free(*sym_table);
  *sym_table = NULL;
};

int __symbol_table_add(SYMBOL_TABLE *sym_table, LL_SYMBOL *symbol)
{
  int key = __htable_insert(sym_table->table, symbol);
  return key;
};

int __symbol_table_find(SYMBOL_TABLE *sym_table, char *symbol)
{
  LL_SYMBOL sym;
  sym.symbol = symbol;
  int key = __htable_find(sym_table->table, &sym);
  return key;
};

int __symbol_table_delete(SYMBOL_TABLE *sym_table, int key)
{
  return __htable_delete(sym_table->table, key);
};

