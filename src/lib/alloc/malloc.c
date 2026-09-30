#include "llpc/lib/alloc/malloc.h"
#include "llpc/lib/alloc/alloc_impl.h"

VOID *llpc_malloc(UINTN size)
{
	if (size == 0)
		return LLPC_NULL;

	LLPC_AllocBlock *block;

	size = llpc_alignUp(size, LLPC_DEFAULT_ALIGNMENT);

	for (block = llpc_allocHeap.first ;
			block != LLPC_NULL ;
			block = block->next)
	{
		if (!block->free || block->size < size)
			continue;

		// If block size is bigger than what the alloc allows:
		// Move forward
		if (block->size >=
				size + sizeof(LLPC_AllocBlock) + LLPC_DEFAULT_ALIGNMENT)
		{
			LLPC_AllocBlock *split =
				(LLPC_AllocBlock*)
				((UINT8*)(block + 1)
				 + size);

			split->size = block->size - size - sizeof(LLPC_AllocBlock);
			split->free = llpctrue;

			split->next = block->next;
			split->prev = block;
		}

		block->free = llpcfalse;

		return (VOID*)(block + 1);
	}

	return LLPC_NULL;
}

