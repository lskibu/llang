# include "util.h"
# include "node.h"


llang_ptr __llang_default_cmp_proc(llang_ptr ptr1, llang_ptr ptr2)
{
     if((llang_i64)  ptr_to_offt(ptr1) < (llang_i64) ptr_to_offt(ptr2))
	     return (llang_ptr) 1;
     else if ((llang_i64) ptr_to_offt(ptr1) == (llang_i64) ptr_to_offt(ptr2))
	     return (llang_ptr) 0;
     else
	     return (llang_ptr) -1;
}

__LLANG_LINKED_LIST * __llang_list_create() 
{
    __LLANG_LINKED_LIST *ll_list = (__LLANG_LINKED_LIST *) llang_malloc(sizeof(__LLANG_LINKED_LIST));
   return ll_list;
};

__LLANG_NODE *__llang_node_create() 
{
    return (__LLANG_NODE *) llang_malloc(sizeof(__LLANG_NODE));
};

void __llang_list_append(__LLANG_LINKED_LIST *ll_list, llang_ptr data_ptr) 
{
    __LLANG_NODE *ll_node = __llang_node_create(); 
    ll_node->data_ptr = data_ptr; 
    if(ll_list->head == NULL) {
	ll_list->head = ll_list->tail = ll_node; 
    } 
    else {
	if(ll_list->head->next == NULL) ll_list->head->next = ll_node;
	ll_node->prev = ll_list->tail; 
	ll_list->tail->next = ll_node; 
	ll_list->tail = ll_node; 
    }; 
    ll_list->length++; 
};
void __llang_list_push(__LLANG_LINKED_LIST *ll_list, llang_ptr data_ptr) 
{
    __LLANG_NODE *ll_node = __llang_node_create(); 
    ll_node->data_ptr = data_ptr; 
    if(ll_list->head == NULL) {
	ll_list->head = ll_list->tail = ll_node;	
    } else {
    	ll_node->next = ll_list->head;
	ll_list->head->prev = ll_node;
	ll_list->head = ll_node;
    }
    ll_list->length++;
};

void __llang_list_delete(__LLANG_LINKED_LIST *ll_list, __LLANG_NODE *ll_node)
{
    if(ll_node == ll_list->head && ll_node == ll_list->tail) {
	ll_list->head = ll_list->tail = NULL;
    } else if(ll_node == ll_list->head) {
	ll_list->head = ll_list->head->next;
	ll_list->head->prev = NULL;
    } else if(ll_node == ll_list->tail) { 
	ll_list->tail = ll_list->tail->prev; 
	ll_list->tail->next = NULL;
    } else {
	ll_node->prev->next = ll_node->next;
	ll_node->next->prev = ll_node->prev;
    }
    free(ll_node);
    ll_list->length--;
};

void __llang_list_clear(__LLANG_LINKED_LIST *ll_list)
{
	for(__LLANG_NODE *ll_node = ll_list->tail; ll_node != NULL; ll_node = ll_node->prev)
		__llang_list_delete(ll_list, ll_node);	
};

llang_i32 __llang_list_indexof(__LLANG_LINKED_LIST *ll_list, __LLANG_NODE *ll_node)
{
	llang_i32 index = 0;
	for(__LLANG_NODE *_node = ll_list->head;
			_node != NULL; 
			_node = _node->next, index++)
		if(_node==ll_node)
			return index;
	return -1;
}

llang_i32 __llang_list_find(__LLANG_LINKED_LIST *ll_list,llang_ptr 
		*data_ptr, llang_lambda cmp_proc)
{
	llang_lambda data_cmp_proc = cmp_proc ? cmp_proc : (llang_lambda) __llang_default_cmp_proc;
	llang_i32 i = 0;
	for(__LLANG_NODE *node = ll_list->head ;
			node != NULL ; 
			node = node->next, i++)
	{
		if( ptr_to_offt(data_cmp_proc(node->data_ptr, data_ptr)) == 0)
			return i;
	}
	return -1;
}

void __llang_list_sort(__LLANG_LINKED_LIST *ll_list, llang_lambda cmp_proc)
{
    llang_lambda data_cmp_proc = cmp_proc ? cmp_proc : (llang_lambda) __llang_default_cmp_proc;

    llang_bool __is_sorted(__LLANG_LINKED_LIST *ll_list)
    {
	if(ll_list->length < 2) return true;
	for(__LLANG_NODE *node = ll_list->head;
			node->next != NULL;
			node = node->next)
	{
	    if((llang_i32)ptr_to_offt(data_cmp_proc(node->data_ptr, node->next->data_ptr)) < 0)
		    return false;
	}
	return true;
    }

    void __insertion_sort(__LLANG_LINKED_LIST *ll_list)
    { 
	if(ll_list->length < 2) return;
	for(__LLANG_NODE *ll_node = ll_list->head->next; ll_node ; ll_node = ll_node->next)
	{
	    __LLANG_NODE *ll_piv_node = ll_node->prev;
	    	while(ll_piv_node && (llang_i32)ptr_to_offt(data_cmp_proc(ll_piv_node->data_ptr, ll_piv_node->next->data_ptr)) < 0)
	    	{
			llang_ptr tmp_ptr = ll_piv_node->data_ptr;
			ll_piv_node->data_ptr = ll_piv_node->next->data_ptr;
			ll_piv_node->next->data_ptr = tmp_ptr;
		        ll_piv_node = ll_piv_node->prev;	
	    	}  
	}
    }

    void __quicksort(__LLANG_LINKED_LIST *ll_list, __LLANG_NODE *ll_head, __LLANG_NODE *ll_tail)
    { 
	if((ll_head == ll_tail) || !ll_head || !ll_tail) return;

	__LLANG_NODE *ll_piv_node = ll_tail;
	__LLANG_NODE *ll_node;
	__LLANG_NODE *ll_prt_node = ll_head;
	for(ll_node = ll_head; ll_node != ll_tail; ll_node = ll_node->next)
	{
	    if( (llang_i32)ptr_to_offt(data_cmp_proc(ll_node->data_ptr, ll_piv_node->data_ptr)) > 0)
	    {
		    llang_ptr tmp_ptr = ll_prt_node->data_ptr;
		    ll_prt_node->data_ptr = ll_node->data_ptr;
		    ll_node->data_ptr = tmp_ptr;
		    ll_prt_node = ll_prt_node->next;
	    }
	}
	llang_ptr tmp_ptr = ll_prt_node->data_ptr;
	ll_prt_node->data_ptr = ll_piv_node->data_ptr;
	ll_piv_node->data_ptr = tmp_ptr;
	
	if(ll_head != ll_prt_node)
	__quicksort(ll_list, ll_head, ll_prt_node->prev);
        if(ll_tail != ll_prt_node)
	__quicksort(ll_list, ll_prt_node->next, ll_tail);	
    };
    
    if((ll_list->length < 2) || __is_sorted(ll_list)) 
    {	printf("sorted\n");
	    return;}
    
    if(ll_list->length <= 25)
	__insertion_sort(ll_list);
    else
	__quicksort(ll_list, ll_list->head, ll_list->tail);
};

llang_u32 __llang_list_remove_duplicates(__LLANG_LINKED_LIST *ll_list, llang_lambda cmp_proc)
{
    llang_lambda data_cmp_proc = cmp_proc ? cmp_proc : (llang_lambda) __llang_default_cmp_proc;
    llang_u32 dup_count = 0;
    if(ll_list->length < 2)
	return dup_count;
     __llang_list_sort(ll_list, cmp_proc);
     for(__LLANG_NODE *ll_node = ll_list->tail; ll_node->prev != NULL; ll_node = ll_node->prev)
     {
	 if(ptr_to_offt(data_cmp_proc(ll_node->data_ptr, ll_node->prev->data_ptr)) == 0) {
		__llang_list_delete(ll_list, ll_node);
		dup_count ++;
	 }
     }
     return dup_count;
}

void __llang_list_destroy(__LLANG_LINKED_LIST **ll_list)
{
    __llang_list_clear(*ll_list);
    free(*ll_list);
    *ll_list = NULL;
}


LLANG_STACK llang_stack_create()
{
    return __llang_list_create();
}

void llang_stack_push(LLANG_STACK ll_stack,llang_ptr ptr)
{
     __llang_list_push(ll_stack, ptr);
}

void llang_stack_pop(LLANG_STACK ll_stack)
{
    __llang_list_delete(ll_stack, ll_stack->head);
}

llang_ptr llang_stack_top(LLANG_STACK ll_stack)
{
    return __llang_list_head(ll_stack)->data_ptr;
}

llang_bool llang_stack_empty(LLANG_STACK ll_stack)
{
    return __llang_list_empty(ll_stack);
}

void llang_stack_destroy(LLANG_STACK *ll_stack)
{
     __llang_list_destroy(ll_stack);
}

LLANG_QUEUE llang_queue_create()
{
    return __llang_list_create();
};

void llang_queue_put(LLANG_QUEUE ll_queue, llang_ptr ptr)
{
    __llang_list_append(ll_queue, ptr);
};

llang_ptr llang_queue_get(LLANG_QUEUE ll_queue)
{
    llang_ptr ret_ptr = __llang_list_head(ll_queue)->data_ptr;
    __llang_list_delete(ll_queue, ll_queue->head);
    return ret_ptr;
};

llang_bool llang_queue_empty(LLANG_QUEUE ll_queue)
{
    return __llang_list_empty(ll_queue);
};

void llang_queue_destroy(LLANG_QUEUE *ll_queue)
{
    __llang_list_destroy(ll_queue);
};

