#include "llpc/lib/alloc/malloc.h"
#include "llpc/lib/alloc/alloc_impl.h"

VOID *llpc_malloc(UINTN size)
{
	if (size == 0)
		return LLPC_NULL;

	size = llpc_alignUp(size, LLPC_DEFAULT_ALIGNMENT);

	for (LLPC_AllocBlock *block = llpc_allocHeap.first ;
			block != LLPC_NULL ;
			block = block->next)
	{
		if (!block->free || block->size < size)
			continue;

		const UINTN remaining = block->size - size;

		if (remaining >=
			sizeof(LLPC_AllocBlock) + LLPC_DEFAULT_ALIGNMENT)
		{
			LLPC_AllocBlock *split =
				(LLPC_AllocBlock *)(
					(UINT8 *)(block + 1) + size);

			split->size =
				remaining - sizeof(LLPC_AllocBlock);

			split->free = llpctrue;

			split->next = block->next;
			split->prev = block;

			if (split->next)
				split->next->prev = split;

			block->next = split;
			block->size = size;
		}

		block->free = llpcfalse;

		return (VOID *)(block + 1);
	}

	return LLPC_NULL;
}

