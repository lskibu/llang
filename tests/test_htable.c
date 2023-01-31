#  include <stdio.h>
#  include <stdlib.h>
#  include <sys/time.h>
#  include <time.h>
#  include "../src/util.h"
#  include "../src/node.h"
#  include "../src/htable.h"

const int N=1e7;
const int MOD = 986743;

int main()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_sec*1000+tv.tv_usec*1000);
	HASH_TABLE *table = __new_hash_table(N, tv.tv_sec*1000+tv.tv_usec/1000 );	

	__init_hash_table(table, NULL, NULL);
	int dups =0;
	LLANG_LIST *list = llang_list_create();
	for(int i=0;i < N; i++) llang_list_append(list, (llang_ptr) (rand()));
	//int ld= llang_list_unique(list, NULL);
	//printf("Removed %d duplicate items from the list\n", ld);
	clock_t c = clock();
	for(__LLANG_NODE *node=list->head;node!=NULL; node=node->next) {
		int n = __htable_insert(table, node->data_ptr);
		if(n == -1) dups++;
	}
	c = clock() - c;
	int col= 0;
	int empty = 0;
	int hashes = 0;
	for(int i=0;i < table->size; i++)
	{
		if(table->items[i]) {
			col += table->items[i]->collisions->length;
			hashes++;
		} else empty++;	
	}
	__htable_destroy(&table);
	printf("Hashes:%d\tEmpty: %d\tCollision: %d\tDups: %d\tCollision rate: %.2f%%\n", hashes, empty, col, dups, (float)col*100/(hashes+col));
	printf("Inserted %d Items in %.5f sec...\n", N, (float)c/CLOCKS_PER_SEC);
	return 0;
}
