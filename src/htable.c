# include <stdio.h>
# include "util.h"
# include "node.h"
# include "htable.h"

static const int MOD = 99999941;
static const int MAX_SIZE = 1e8+5;

static llang_ptr __def_data_to_i32(llang_ptr ptr)
{
	return (llang_ptr) ptr_to_offt(ptr);
};

static llang_ptr __def_cmp_proc(llang_ptr ptr1, llang_ptr ptr2) {
	if ((llang_i64) ptr_to_offt(ptr1) < (llang_i64)ptr_to_offt(ptr2)) return (llang_ptr) 1;
	else if((llang_i64)ptr_to_offt(ptr1) > (llang_i64)ptr_to_offt(ptr2)) return (llang_ptr) -1;
	else return NULL;
}
static int compute_hash(HASH_TABLE *table, llang_ptr data_ptr) {
	return ((table->salt ^ ((llang_i32) ptr_to_offt(
			table->data_to_i32(data_ptr))) ) 
			% MOD) % table->size;
};

struct TABLE_ITEM *__new_table_item(llang_ptr data_ptr)
{
    struct TABLE_ITEM *item = llang_malloc(sizeof(struct TABLE_ITEM));
    item->data_ptr = data_ptr;
    item->collisions = llang_list_create();
    return item;
};
void __delete_table_item(struct TABLE_ITEM **item) 
{
	llang_list_destroy((*item)->collisions);
	free(*item);
	*item = NULL;
};

HASH_TABLE *__new_hash_table(int size, int salt)
{
	llang_assert(size > 0 && size < MAX_SIZE && "Invalid size");
	HASH_TABLE *table = (HASH_TABLE *) llang_malloc(sizeof(HASH_TABLE));
	table->size = size;
	table->salt = salt;
	return table;
};
void __init_hash_table(HASH_TABLE *table, llang_lambda cmp_proc, llang_lambda data_to_i32)
{
	table->items = (struct TABLE_ITEM **) llang_calloc(table->size, sizeof(struct TABLE_ITEM *));
	table->cmp_proc = cmp_proc ? cmp_proc : __def_cmp_proc;
	table->data_to_i32 = data_to_i32 ? data_to_i32 : __def_data_to_i32;
};

int __htable_insert(HASH_TABLE *table, llang_ptr data_ptr)
{
	struct TABLE_ITEM *item = __new_table_item(data_ptr);
	item->data_ptr = data_ptr;
	int hash = compute_hash(table, data_ptr);
	if(table->items[hash] != NULL) {
		if(ptr_to_offt(table->cmp_proc(table->items[hash]->data_ptr, 
			data_ptr)) == 0) { __delete_table_item(&item); return -1; }
		else if(llang_list_find(table->items[hash]->collisions, item->data_ptr, table->cmp_proc) >= 0) {
				printf("duplicate collision found\n");
				__delete_table_item(&item); return -1;
		} else llang_list_append(table->items[hash]->collisions, item); 
	} else table->items[hash] = item;

	return hash;
}
;
int __htable_delete(HASH_TABLE *table, int index)
{
	if(table->items[index]) {
		for(__LLANG_NODE *node=table->items[index]->collisions->head; node!=NULL; node = node->next)
			__delete_table_item((struct TABLE_ITEM **)&node->data_ptr);
		__delete_table_item(&table->items[index]);
		return index;
	} else return -1;
}
int __htable_find(HASH_TABLE *table, llang_ptr data_ptr)
{
	int hash = compute_hash(table, data_ptr);
	if(table->items[hash]) return hash;
	else return -1;
}	
;

void __htable_destroy(HASH_TABLE **table)
{
	for(int i=0;i < (*table)->size; i++) {
	    if((*table)->items[i]) {
		__delete_table_item(&(*table)->items[i]);
	    }
	}
	free((*table)->items);
	free(*table);
	*table = NULL;
}
