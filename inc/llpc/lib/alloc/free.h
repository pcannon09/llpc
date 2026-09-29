#ifndef INCLUDE_ALLOC_FREE_H_
#define INCLUDE_ALLOC_FREE_H_

#include "llpc/lib/types.h"

#ifdef __cplusplus
# 	define LLPC_CPP_FREE_OPEN 		extern "C" {
# 	define LLPC_CPP_FREE_CLOSE 		}
#else
# 	define LLPC_CPP_FREE_OPEN
# 	define LLPC_CPP_FREE_CLOSE
#endif

LLPC_CPP_FREE_OPEN

llpc_bool llpc_ffree(void *ptr);
llpc_bool llpc_impl_free(void **ptr);

#define llpc_free(_ptr) \
	llpc_impl_free((void**)&_ptr)

LLPC_CPP_FREE_CLOSE

#endif  // INCLUDE_ALLOC_FREE_H_

