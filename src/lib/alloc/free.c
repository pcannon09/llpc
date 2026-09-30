#include "llpc/lib/alloc/free.h"
#include "llpc/lib/alloc/alloc_impl.h"

llpc_bool llpc_free(VOID *ptr)
{
	if (!ptr)
		return llpcfalse;

	LLPC_AllocBlock *block;

	block = ((LLPC_AllocBlock*)ptr) - 1;
	block->free = llpctrue;

	llpc_heapMerge(block);

	return llpctrue;
}

