#ifndef INCLUDE_ALLOC_ALLOCIMPL_H_
#define INCLUDE_ALLOC_ALLOCIMPL_H_

#include <efi/efi.h>
#include <efi/efilib.h>

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

#ifndef LLPC_HEAP_PAGE_SIZE
# 	define LLPC_HEAP_PAGE_SIZE 		EFI_PAGE_SIZE
#endif

#ifndef LLPC_DEFAULT_ALIGNMENT
# 	define LLPC_DEFAULT_ALIGNMENT 	16
#endif

typedef struct LLPC_AllocBlock
{
	// Current block
	UINTN size;
	llpc_bool free; // Is it a free block?

	// Allocation blocks
	struct LLPC_AllocBlock *next;
	struct LLPC_AllocBlock *prev;
} LLPC_AllocBlock;

typedef struct LLPC_AllocHeap
{
	VOID *base;
	UINTN size;

	LLPC_AllocBlock *first;
} LLPC_AllocHeap;

extern LLPC_AllocHeap llpc_allocHeap;

EFI_STATUS llpc_alloc_heapInit(const UINTN pages);

void llpc_heapMerge(LLPC_AllocBlock *block);

static inline UINTN llpc_alignUp(const UINTN value, const UINTN alignment)
{ return (value + alignment - 1) & ~(alignment - 1); }

LLPC_CPP_ALLOCIMPL_CLOSE

#endif  // INCLUDE_ALLOC_ALLOCIMPL_H_

