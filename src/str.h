#ifndef LLANG_STR
#define LLANG_STR

# include <string.h>
# include "util.h"

# define _STR_SIZ 256

typedef struct __STR {
	llang_i32 len;
	llang_i32 siz;
	llang_str *str;
} LLANG_STR;

LLANG_STR *__ll_new_str(llang_str *buf);
void __ll_str_free(LLANG_STR *str);
void __ll_str_copy(LLANG_STR *dst, LLANG_STR *src);
void __ll_str_cat(LLANG_STR *dst, LLANG_STR *src);
void __ll_str_clear(LLANG_STR *dst);
LLANG_STR *__ll_str_substr(LLANG_STR *str, llang_i32 pos, llang_i32 len);
void __ll_str_trim(LLANG_STR *str);
llang_i32 __ll_str_len(LLANG_STR *str);


#endif 
