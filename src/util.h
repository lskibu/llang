#ifndef __LLANG_UTIL__
#define __LLANG_UTIL__

#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  include <errno.h>
#  include <sys/types.h>

# define __llang_system_failure()  { \
	fprintf(stderr, "SYSTEM FAILURE: in %s:%s(), ERROR: %s\n",\
			__FILE__,  __FUNCTION__, strerror(errno)); \
	exit(EXIT_FAILURE);\
	} ;
# define __llang_program_failure(msg) {\
	fprintf(stderr, "PROGRAM FAILURE: in %s:%s(), ERROR: %s\n",\
			__FILE__, __FUNCTION__, msg); \
	exit(EXIT_FAILURE); \
	} ;

# define llang_failure(msg) do {\
	    if(errno != 0) {\
	        __llang_system_failure(); \
	    } else {\
		__llang_program_failure(msg);} \
	} while (0);

#  define llang_assert(expr) {if(! (expr)) llang_failure( #expr );}

#define llang_critical_failure(msg) do {\
	write(STDERR_FILENO, "critical failure\n", 17); \
	_exit (EXIT_FAILURE); \
	};

# define llang_clean_errno() {errno = 0;};

# define ptr_to_offt(ptr) ((llang_offt) ((llang_u8 *)(ptr) - (llang_u8 *)NULL))

#define true 	(1)
#define false 	(0)

/*
 * type aliasing for llang
 */

typedef unsigned char llang_byte;
typedef llang_byte llang_bool;
typedef void * llang_ptr;

typedef char llang_char;
typedef char * llang_str;

typedef char llang_i8;
typedef unsigned char llang_u8;

typedef short llang_i16;
typedef unsigned short llang_u16;

typedef int llang_i32;
typedef unsigned int llang_u32;

typedef long long int llang_i64;
typedef unsigned long long int llang_u64;

# if __WORDSIZE == 64
typedef llang_u64 llang_offt;
# else
typedef llang_u32 llang_offt;
# endif

typedef float llang_float32;
typedef double llang_float64;

typedef llang_bool (*llang_cmp_func)(const llang_ptr,const llang_ptr);
typedef llang_ptr (*llang_lambda)(llang_ptr data_ptr, ...);

/*
 * safe memory allocation functions
 */

llang_ptr llang_malloc(llang_u32 size);

llang_ptr llang_calloc(llang_u32 number_of_blocks, llang_u32 block_size);

llang_ptr llang_realloc(llang_ptr block, llang_u32 new_size);

llang_ptr llang_realloc_arr(llang_ptr block, llang_u32 new_length,
		llang_u32 new_item_size);

#endif
