#  include <stdio.h>
#  include <stdlib.h>
#  include <sys/time.h>
# include "../src/util.h"
# include "../src/node.h"
# include "../src/htable.h"


int main()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    HASH_TABLE *table = __new_hash_table(10111, tv.tv_sec*1000+tv.tv_usec/1000);
    __init_hash_table(table, NULL, NULL);
    int dups, col, hash;
    hash=dups=col=0;
    for(int i=0;i < 10111;i++) {
	srand(i^rand());
    	int r=__htable_insert(table, (void *) rand());
	if(r==-1)dups++;
    }
    for(int i=0;i < table->size;i++) {
	if(table->items[i]) {
		col += table->items[i]->collisions->length;
		hash++;
	}
    }
    printf("Hashes : %d\tDups: %d\tCol: %d\tCollision rate: %.2f%%\n",
		   hash, dups, col, (float) (col*100)/ (hash+col));
    return 0;
}
