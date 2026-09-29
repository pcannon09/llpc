#include "llpc/lib/alloc/malloc.h"
#include "llpc/lib/alloc/alloc_impl.h"

void *llpc_malloc(size_t size)
{
	if (!size)
		return NULL;

	size = LLPC_ALLOC_ALIGN(size);

	LLPC_Alloc_Block *blk = llpc_alloc_findFree(size);

	if (blk)
	{
		blk->free = 0;

		return blk + 1;
	}

	blk = llpc_alloc_createBlock(size);

	if (!blk)
		return LLPC_NULL;

	return blk + 1;
}

