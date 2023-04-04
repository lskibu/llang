# include <stdlib.h>
# include <malloc.h>
# include "util.h"

llang_ptr llang_malloc(llang_u32 size)
{
    llang_ptr mem_block = calloc(1, size);
    llang_assert(mem_block != NULL);
    return mem_block;
}

llang_ptr llang_calloc(llang_u32 number_of_blocks, llang_u32 block_size)
{
    llang_ptr mem_block = calloc(number_of_blocks, block_size);
    llang_assert(mem_block != NULL);
    return mem_block;
}

llang_ptr llang_realloc(llang_ptr block, llang_u32 new_size)
{
    llang_ptr mem_block = realloc(block, new_size);
    llang_assert(block != NULL);
    return mem_block;
}

llang_ptr llang_realloc_arr(llang_ptr block, llang_u32 new_length,
		llang_u32 new_item_size)
{
    llang_ptr mem_block = reallocarray(block, new_length, new_item_size);
    llang_assert(mem_block != NULL);
    return mem_block;
}

