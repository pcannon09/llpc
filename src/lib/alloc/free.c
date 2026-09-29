#include "llpc/lib/alloc/free.h"
#include "llpc/lib/alloc/alloc_impl.h"

llpc_bool llpc_ffree(void *ptr)
{
	if (!ptr)
		return llpcfalse;

	LLPC_Alloc_Block *blk = (LLPC_Alloc_Block*)ptr - 1;

	if (!blk)
		return llpcfalse;

	blk->free = 1;

	return llpctrue;
}

llpc_bool llpc_impl_free(void **ptr)
{
	if (!ptr || !*ptr)
		return llpcfalse;

	const llpc_bool freeRes =
		llpc_ffree(ptr);

	// Return early if failed
	// Do not set it to `LLPC_NULL`
	if (!freeRes)
		return llpcfalse;

	*ptr = LLPC_NULL;

	return freeRes;
}

