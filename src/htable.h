# ifndef __LL_HTABLE__
# define __LL_HTABLE__

# include "util.h"
# include "node.h"

struct TABLE_ITEM {
	llang_ptr data_ptr;
	LLANG_LIST *collisions;
};

struct HASH_TABLE {
	int size;
	int salt;
	llang_lambda cmp_proc;
	llang_lambda data_to_i32;
	struct TABLE_ITEM **items;
} ;

typedef struct HASH_TABLE HASH_TABLE;

struct TABLE_ITEM *__new_table_item(llang_ptr data_ptr);
void __delete_table_item(struct TABLE_ITEM **item);

HASH_TABLE *__new_hash_table(int size, int salt);
void __init_hash_table(HASH_TABLE *table, llang_lambda cmp_proc, llang_lambda data_to_i32);
int __htable_insert(HASH_TABLE *table, llang_ptr data_ptr);
int __htable_delete(HASH_TABLE *table, int index);
int __htable_find(HASH_TABLE *table, llang_ptr data_ptr);
void __htable_destroy(HASH_TABLE **table);


# endif 
