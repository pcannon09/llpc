#ifndef INCLUDE_ALLOC_MALLOC_H_
#define INCLUDE_ALLOC_MALLOC_H_

#include <stddef.h>

#ifdef __cplusplus
# 	define LLPC_CPP_MALLOC_OPEN 		extern "C" {
# 	define LLPC_CPP_MALLOC_CLOSE 		}
#else
# 	define LLPC_CPP_MALLOC_OPEN
# 	define LLPC_CPP_MALLOC_CLOSE
#endif

LLPC_CPP_MALLOC_OPEN

void *llpc_malloc(size_t size);

LLPC_CPP_MALLOC_CLOSE

#endif  // INCLUDE_ALLOC_MALLOC_H_

