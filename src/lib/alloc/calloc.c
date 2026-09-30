#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/alloc/malloc.h"

VOID *llpc_calloc(UINTN count, UINTN size)
{
	UINT8 *ptr;
	UINTN total;
	UINTN i;

	if (count != 0 && size > (~(UINTN)0) / count)
		return NULL;

	total = count * size;

	if (total == 0)
		return NULL;

	ptr = llpc_malloc(total);

	if (!ptr)
		return NULL;

	for (i = 0; i < total; i++)
		ptr[i] = 0;

	return ptr;
}

