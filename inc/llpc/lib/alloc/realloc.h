#ifndef INCLUDE_ALLOC_REALLOC_H_
#define INCLUDE_ALLOC_REALLOC_H_

#include "llpc/lib/alloc/alloc_impl.h"

#ifdef __cplusplus
# 	define LLPC_CPP_REALLOC_OPEN 		extern "C" {
# 	define LLPC_CPP_REALLOC_CLOSE 		}
#else
# 	define LLPC_CPP_REALLOC_OPEN
# 	define LLPC_CPP_REALLOC_CLOSE
#endif

LLPC_CPP_REALLOC_OPEN

void *llpc_impl_alloc_realloc(void *ptr, size_t newSize,
		enum LLPC_Alloc_Method allocMethod);

#define llpc_recalloc(_ptr, _newSize) \
	llpc_impl_alloc_realloc(_ptr, _newSize, LLPC_ALLOC_METHOD_CALLOC)

#define llpc_remalloc(_ptr, _newSize) \
	llpc_impl_alloc_realloc(_ptr, _newSize, LLPC_ALLOC_METHOD_MALLOC)

#define llpc_realloc 		llpc_recalloc

LLPC_CPP_REALLOC_CLOSE

#endif  // INCLUDE_ALLOC_REALLOC_H_

