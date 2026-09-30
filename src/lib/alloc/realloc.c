#include "llpc/lib/alloc/realloc.h"
#include "llpc/lib/alloc/alloc_impl.h"
#include "llpc/lib/alloc/free.h"
#include "llpc/lib/alloc/malloc.h"

VOID *llpc_realloc(VOID *ptr, UINTN size)
{
	LLPC_AllocBlock *block;
	VOID *newPtr;
	UINTN copySize;

	if (ptr == NULL)
		return llpc_malloc(size);

	if (size == 0)
	{
		llpc_nullify(ptr);
		return NULL;
	}

	block = ((LLPC_AllocBlock*)ptr) - 1;

	if (size <= block->size)
		return ptr;

	newPtr = llpc_malloc(size);

	if (!newPtr)
		return NULL;

	copySize = block->size;

	CopyMem(newPtr, ptr, copySize);

	llpc_nullify(ptr);

	return newPtr;
}

