# include <stdio.h>
# include <sys/time.h>
#  include <malloc.h>
# include <time.h>
# include "../src/util.h"
# include "../src/node.h"


const int N = 1e7+7;
const int MOD = 986743;

void print_list(__LLANG_LINKED_LIST *);

int main()
{
	LLANG_LIST *ll_list = llang_list_create();
	struct timeval tv;
	gettimeofday(&tv, NULL);
    srand(tv.tv_sec*1000 + tv.tv_usec / 1000);
    clock_t ct = clock();
    for(llang_i32 I=0; I< N; I++)
    {
	llang_list_append(ll_list,(llang_ptr) (rand() % (llang_i64)MOD));
	//llang_list_push(ll_list,(llang_ptr) (rand() % (llang_i64)1e4+7));
    }
    ct = clock() - ct;
    printf("List contains: %d\n", ll_list->length);
    //printf("+++++++++++++++++++++++++++++++++++++++\n");
    //print_list(ll_list);
    //printf("+++++++++++++++++++++++++++++++++++++++\n");
    clock_t c = clock();
    //printf("dups: %d\n", llang_list_unique(ll_list, NULL));
    llang_list_sort(ll_list, NULL);
    printf("llang_list_sort took %.5f sec\n", ((float) clock()-c)/CLOCKS_PER_SEC);
    printf("Inserted %d Items in %.5f sec\n", N, (float)ct/CLOCKS_PER_SEC);
    //print_list(ll_list);
    int ret = llang_list_find(ll_list, 54, NULL);
    printf("number found at index %d\n", ret);
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

