#ifndef INCLUDE_ALLOC_ALLOCIMPL_H_
#define INCLUDE_ALLOC_ALLOCIMPL_H_

#include <stddef.h>

#include "llpc/lib/types.h"

#ifdef __cplusplus
# 	define LLPC_CPP_ALLOCIMPL_OPEN 		extern "C" {
# 	define LLPC_CPP_ALLOCIMPL_CLOSE 		}
#else
# 	define LLPC_CPP_ALLOCIMPL_OPEN
# 	define LLPC_CPP_ALLOCIMPL_CLOSE
#endif

LLPC_CPP_ALLOCIMPL_OPEN

typedef struct LLPC_Alloc_Block
{
	struct LLPC_Alloc_Block *next;
	struct LLPC_Alloc_Block *prev;

	size_t size;
	int free;
} LLPC_Alloc_Block;

static LLPC_Alloc_Block *__llpc_alloc_head = LLPC_NULL;
static LLPC_Alloc_Block *__llpc_alloc_tail = LLPC_NULL;

static void *__llpc_alloc_heapEnd = LLPC_NULL;

#ifndef LLPC_ALLOC_ALIGNMENT
# 	define LLPC_ALLOC_ALIGNMENT 		16
#endif

#define LLPC_ALLOC_ALIGN(x) 		(((x) + LLPC_ALLOC_ALIGNMENT - 1) & ~((size_t)LLPC_ALLOC_ALIGNMENT - 1))

enum LLPC_Alloc_Method
{
	LLPC_ALLOC_METHOD_CALLOC,
	LLPC_ALLOC_METHOD_MALLOC
};

void *llpc_alloc_sbrk(const size_t increment);

LLPC_Alloc_Block *llpc_alloc_findFree(const size_t size);
LLPC_Alloc_Block *llpc_alloc_createBlock(const size_t size);

llpc_bool llpc_alloc_mergeNextBlk(LLPC_Alloc_Block *blk);
llpc_bool llpc_alloc_canExpandPlace(const LLPC_Alloc_Block *blk, const size_t newSize);

LLPC_CPP_ALLOCIMPL_CLOSE

#endif  // INCLUDE_ALLOC_ALLOCIMPL_H_

