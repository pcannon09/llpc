#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/alloc/malloc.h"

#include "llpc/lib/types.h"

VOID *llpc_calloc(UINTN count, UINTN size)
{
	if (count == 0 || size == 0)
		return LLPC_NULL;

	if (size > (~(UINTN)0) / count)
		return LLPC_NULL;

	const UINTN total = count * size;

	UINT8 *ptr = llpc_malloc(total);

	if (!ptr)
		return LLPC_NULL;

	for (UINTN i = 0 ; i < total ; i++)
		ptr[i] = 0;

	return ptr;
}

