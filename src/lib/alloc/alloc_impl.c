#include "llpc/lib/alloc/alloc_impl.h"

extern void *__llpc_sysBrk(void *addr);

void *llpc_alloc_sbrk(const size_t increment)
{
	if (!__llpc_alloc_heapEnd)
		__llpc_alloc_heapEnd = __llpc_sysBrk(0);

	void *old = __llpc_alloc_heapEnd;
	void *new = (char*)old + increment;

	if (__llpc_sysBrk(new) != new)
		return LLPC_NULL;

	__llpc_alloc_heapEnd = new;

	return old;
}

LLPC_Alloc_Block *llpc_alloc_findFree(const size_t size)
{
	LLPC_Alloc_Block *blk = __llpc_alloc_head;

	while (blk)
	{
		if (blk->free && blk->size >= size)
			return blk; // Found free space

		blk = blk->next;
	}

	return NULL;
}

LLPC_Alloc_Block *llpc_alloc_createBlock(const size_t size)
{
	LLPC_Alloc_Block *blk = llpc_alloc_sbrk(sizeof(LLPC_Alloc_Block) + size);

	if (!blk)
		return NULL;

	blk->size = size;
	blk->free = 0;
	blk->next = NULL;

	if (!__llpc_alloc_head)
		__llpc_alloc_head = blk;

	else __llpc_alloc_tail->next = blk;

	__llpc_alloc_tail = blk;

	return blk;
}

llpc_bool llpc_alloc_canExpandPlace(const LLPC_Alloc_Block *blk, const size_t newSize)
{
	if (!blk || newSize == 0)
		return llpcfalse;

	LLPC_Alloc_Block *next = blk->next;

	if (!next)
		return llpcfalse;

	if (!next->free)
		return llpcfalse;

	const size_t total =
		blk->size + sizeof(LLPC_Alloc_Block) +
		next->size;

	return total >= newSize;
}

llpc_bool llpc_alloc_mergeNextBlk(LLPC_Alloc_Block *blk)
{
	if (!blk)
		return llpcfalse;

	LLPC_Alloc_Block *next = blk->next;

	if (!next)
		return llpcfalse;

	blk->size += sizeof(LLPC_Alloc_Block) + next->size;
	blk->next = next->next;

	if (__llpc_alloc_tail == next)
		__llpc_alloc_tail = blk;

	else return llpcfalse;

	return llpctrue;
}

