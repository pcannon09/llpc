#ifndef INCLUDE_ALLOC_CALLOC_H_
#define INCLUDE_ALLOC_CALLOC_H_

#include <stddef.h>

#include "llpc/lib/types.h"

#ifdef __cplusplus
# 	define LLPC_CPP_CALLOC_OPEN 		extern "C" {
# 	define LLPC_CPP_CALLOC_CLOSE 		}
#else
# 	define LLPC_CPP_CALLOC_OPEN
# 	define LLPC_CPP_CALLOC_CLOSE
#endif

LLPC_CPP_CALLOC_OPEN

void *llpc_calloc(const size_t nmemb, const size_t size);

LLPC_CPP_CALLOC_CLOSE

#endif  // INCLUDE_ALLOC_CALLOC_H_

