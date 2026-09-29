#ifndef INCLUDE_LIB_TYPES_H_
#define INCLUDE_LIB_TYPES_H_

#include "llpc/predefines.h"

#ifdef __cplusplus
#	define LLPC_TYPES_OPEN			extern "C" {
#	define LLPC_TYPES_CLOSE		}
#else
#	define LLPC_TYPES_OPEN
#	define LLPC_TYPES_CLOSE
#endif

LLPC_TYPES_OPEN

typedef enum llpc_bool
{
	llpcfalse = 0,
	llpctrue,
	llpcnone,
} llpc_bool;

typedef enum LLPC_SystemError
{
	LLPC_SE_OK,

	LLPC_SE_InitNull,

	LLPC_SE_UNKNOWN,
} LLPC_SystemError;

#if __LLPC_HAS_C23
# 	define LLPC_NULL nullptr
#else
# 	define LLPC_NULL ((void*)0)
#endif

LLPC_TYPES_CLOSE

#endif  // INCLUDE_LIB_TYPES_H_
