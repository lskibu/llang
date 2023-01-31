#ifndef __LLANG_NODE__
#define __LLANG_NODE__

# include "util.h"


/*
 * List data structure implemented using pointers
 *
 */


# define llang_list_create() 			__llang_list_create()
# define llang_list_destroy(list) 		__llang_list_destroy(&list)
# define llang_list_append(list, ptr)		__llang_list_append(list, ptr)
# define llang_list_push(list, ptr)		__llang_list_push(list, ptr)
# define llang_list_clear(list)			__llang_list_clear(list, ptr)
# define llang_list_delete(list, node)		__llang_list_delete(list, node)
# define llang_list_indexof(list, node) 	__llang_list_indexof(list, node)
# define llang_list_find(list, ptr, lambda)	__llang_list_find(list, ptr, lambda)
# define llang_list_sort(list, cmp_proc)	__llang_list_sort(list, cmp_proc)
# define llang_list_unique(list, cmp_proc)	__llang_list_remove_duplicates(list, cmp_proc)


typedef struct __LLANG_NODE {
	llang_ptr data_ptr;
	struct __LLANG_NODE *next;
	struct __LLANG_NODE *prev;
} __LLANG_NODE;

typedef struct __LLANG_LINKED_LIST {
    llang_i32 length;
    __LLANG_NODE *head;
    __LLANG_NODE *tail;
} __LLANG_LINKED_LIST;


typedef __LLANG_LINKED_LIST LLANG_LIST;

llang_ptr __llang_default_cmp_proc(llang_ptr ptr1, llang_ptr ptr2);

__LLANG_LINKED_LIST * __llang_list_create();

__LLANG_NODE *__llang_node_create();

void __llang_list_append(__LLANG_LINKED_LIST *ll_list, llang_ptr data_ptr);
void __llang_list_push(__LLANG_LINKED_LIST *ll_list, llang_ptr data_ptr);

# define __llang_list_empty(ll_list) (ll_list->head == NULL)
# define __llang_list_head(ll_list) (ll_list->head)
# define __llang_list_tail(ll_list) (ll_list->tail)

void __llang_list_delete(__LLANG_LINKED_LIST *ll_list, __LLANG_NODE *ll_node);

void __llang_list_clear(__LLANG_LINKED_LIST *ll_list);

llang_i32 __llang_list_indexof(__LLANG_LINKED_LIST *ll_list, __LLANG_NODE *ll_node);

llang_i32 __llang_list_find(__LLANG_LINKED_LIST *ll_list,llang_ptr 
		*data_ptr, llang_lambda cmp_proc);

void __llang_list_sort(__LLANG_LINKED_LIST *ll_list, llang_lambda cmp_proc);

llang_u32 __llang_list_remove_duplicates(__LLANG_LINKED_LIST *ll_list, llang_lambda cmp_proc);

void __llang_list_destroy(__LLANG_LINKED_LIST **ll_list);

/* Stack data structure
 */

typedef __LLANG_LINKED_LIST * LLANG_STACK;

LLANG_STACK llang_stack_create();

void llang_stack_push(LLANG_STACK ll_stack,llang_ptr ptr);

void llang_stack_pop(LLANG_STACK ll_stack);

llang_ptr llang_stack_top(LLANG_STACK ll_stack);

llang_bool llang_stack_empty(LLANG_STACK ll_stack);

void llang_stack_destroy(LLANG_STACK *ll_stack);

/* Queue data structure
 */
typedef __LLANG_LINKED_LIST * LLANG_QUEUE;

LLANG_QUEUE llang_queue_create();

void llang_queue_put(LLANG_QUEUE ll_queue, llang_ptr ptr);

llang_ptr llang_queue_get(LLANG_QUEUE ll_queue);

llang_bool llang_queue_empty(LLANG_QUEUE ll_queue);

void llang_queue_destroy(LLANG_QUEUE *ll_queue);


#endif
