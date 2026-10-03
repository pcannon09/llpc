#include "llpc/lib/alloc/alloc.h"
#include "llpc/lib/alloc/alloc_impl.h"

UINTN llpc_allocSize(const VOID *ptr)
{
	if (ptr == LLPC_NULL)
		return 0;

	const LLPC_AllocBlock *block = ((const LLPC_AllocBlock *)ptr) - 1;

	return block->size;
}

