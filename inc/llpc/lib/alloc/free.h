#ifndef INCLUDE_ALLOC_FREE_H_
#define INCLUDE_ALLOC_FREE_H_

#include <efi/efi.h>

#include "llpc/lib/types.h"

#ifdef __cplusplus
# 	define LLPC_CPP_FREE_OPEN 		extern "C" {
# 	define LLPC_CPP_FREE_CLOSE 		}
#else
# 	define LLPC_CPP_FREE_OPEN
# 	define LLPC_CPP_FREE_CLOSE
#endif

LLPC_CPP_FREE_OPEN

llpc_bool llpc_free(VOID *ptr);

#define llpc_nullify(_ptr) 		\
	do {						\
		llpc_free(_ptr); 		\
		_ptr = LLPC_NULL; 		\
	} while (0)

LLPC_CPP_FREE_CLOSE

#endif  // INCLUDE_ALLOC_FREE_H_

