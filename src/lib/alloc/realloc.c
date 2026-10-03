#include "llpc/lib/alloc/realloc.h"
#include "llpc/lib/alloc/alloc_impl.h"
#include "llpc/lib/alloc/free.h"
#include "llpc/lib/alloc/malloc.h"

VOID *llpc_realloc(VOID *ptr, UINTN size)
{
	LLPC_AllocBlock *block;
	VOID *newPtr;
	UINTN copySize;

	if (ptr == LLPC_NULL)
		return llpc_malloc(size);

	if (size == 0)
	{
		llpc_nullify(ptr);
		return LLPC_NULL;
	}

	// Align before comparing against the already-aligned `block->size`
	const UINTN alignedSize =
		llpc_alignUp(size, LLPC_DEFAULT_ALIGNMENT);

	block = ((LLPC_AllocBlock *)ptr) - 1;

	if (alignedSize <= block->size)
		return ptr;

	newPtr = llpc_malloc(size);

	if (!newPtr)
		return LLPC_NULL;

	copySize = block->size;

	CopyMem(newPtr, ptr, copySize);

	llpc_nullify(ptr);

	return newPtr;
}

