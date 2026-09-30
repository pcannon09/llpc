#ifndef INCLUDE_ALLOC_REALLOC_H_
#define INCLUDE_ALLOC_REALLOC_H_

#include <efi/efi.h>

#ifdef __cplusplus
# 	define LLPC_CPP_REALLOC_OPEN 		extern "C" {
# 	define LLPC_CPP_REALLOC_CLOSE 		}
#else
# 	define LLPC_CPP_REALLOC_OPEN
# 	define LLPC_CPP_REALLOC_CLOSE
#endif

LLPC_CPP_REALLOC_OPEN

VOID *llpc_realloc(VOID *ptr, UINTN size);

LLPC_CPP_REALLOC_CLOSE

#endif  // INCLUDE_ALLOC_REALLOC_H_

