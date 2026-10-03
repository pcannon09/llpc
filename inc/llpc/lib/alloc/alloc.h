#ifndef INCLUDE_ALLOC_ALLOC_H_
#define INCLUDE_ALLOC_ALLOC_H_

#include <efi/x86_64/efibind.h>

#ifdef __cplusplus
# 	define LLPC_CPP_ALLOC_OPEN 		extern "C" {
# 	define LLPC_CPP_ALLOC_CLOSE 		}
#else
# 	define LLPC_CPP_ALLOC_OPEN
# 	define LLPC_CPP_ALLOC_CLOSE
#endif

LLPC_CPP_ALLOC_OPEN

UINTN llpc_allocSize(const VOID *ptr);

LLPC_CPP_ALLOC_CLOSE

#endif  // INCLUDE_ALLOC_ALLOC_H_
