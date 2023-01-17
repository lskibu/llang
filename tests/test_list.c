# include <stdio.h>
# include <sys/time.h>
#  include <malloc.h>
# include <time.h>
# include "../src/util.h"
# include "../src/node.h"


void print_list(__LLANG_LINKED_LIST *);

int main()
{
	LLANG_LIST *ll_list = llang_list_create();
	struct timeval tv;
	gettimeofday(&tv, NULL);
    srand(tv.tv_sec*1000 + tv.tv_usec / 1000);
    for(llang_i32 I=0; I< 1e6; I++)
    {
	llang_list_append(ll_list,(llang_ptr) (rand() % (llang_i64)1e4+7));
	llang_list_push(ll_list,(llang_ptr) (rand() % (llang_i64)1e4+7));
    }

    printf("List contains: %d\n", ll_list->length);
    printf("+++++++++++++++++++++++++++++++++++++++\n");
    print_list(ll_list);
    printf("+++++++++++++++++++++++++++++++++++++++\n");
    clock_t c = clock();
    printf("dups: %d\n",__llang_remove_duplicates(ll_list, NULL));
    printf("took %.5f sec\n", ((float) clock()-c)/CLOCKS_PER_SEC);
    print_list(ll_list);
    llang_list_destroy(ll_list);
    return 0;
}

void print_list(__LLANG_LINKED_LIST *ll_list)
{
if(ll_list->length < 100) {
   __LLANG_NODE *ll_node = ll_list->head;
for(llang_i32 I=0;I < ll_list->length; I++)
{
printf("%lld ", (llang_i64)ll_node->data_ptr);
ll_node = ll_node->next;

if((I+1) % 10 == 0) printf("\n");
}
} else {
	__LLANG_NODE *node = ll_list->head;
for(llang_i32 I=0;I < 20; I++) {
		printf("%lld ", (llang_i64)node->data_ptr);
		node = node->next;
		if((I+1) % 10 == 0)
			printf("\n");
	}
	printf("............\n");
	node = ll_list->tail;
	for(llang_i32 I=ll_list->length-21; I < ll_list->length; I++)
	{
		printf("%lld ",(llang_i64) node->data_ptr);
		node = node->prev;
		if((I+1) % 10 == 0)
			printf("\n");
	}
}
printf("\n");
}

