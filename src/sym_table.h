# ifndef LLANG_SYM_TABLE
# define LLANG_SYM_TABLE

# include "util.h"
# include "htable.h"

typedef enum {
  SYM_VAR,
  SYM_UNIT,
  SYM_STRUCT,
  SYM_CLASS,
  SYM_PROC
} LL_T_SYM;

typedef struct LL_SYMBOL {
  char *symbol;
  LL_T_SYM type ;
} LL_SYMBOL;

typedef struct SYMBOL_TABLE {
  char *unit;
  HASH_TABLE *table;
} SYMBOL_TABLE;

LL_SYMBOL *__new_symbol(char *symbol,LL_T_SYM type);
void __del_symbol(LL_SYMBOL **symbol);


SYMBOL_TABLE *__new_symbol_table(char *unit);
void __del_symbol_table(SYMBOL_TABLE **sym_table);
int __symbol_table_add(SYMBOL_TABLE *sym_table, LL_SYMBOL *symbol);
int __symbol_table_find(SYMBOL_TABLE *sym_table, char *symbol);
int __symbol_table_delete(SYMBOL_TABLE *sym_table, int key);

# endif

