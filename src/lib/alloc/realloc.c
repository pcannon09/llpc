#include "llpc/lib/alloc/realloc.h"
#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/alloc/free.h"
#include "llpc/lib/alloc/malloc.h"

#include "llpc/lib/string/string.h"

void *llpc_impl_alloc_realloc(void *ptr, size_t newSize,
		enum LLPC_Alloc_Method allocMethod)
{
	if (!ptr || newSize == 0)
		return LLPC_NULL;

	newSize = LLPC_ALLOC_ALIGN(newSize);

	LLPC_Alloc_Block *blk = (LLPC_Alloc_Block*)ptr - 1;

	if (!blk)
		return NULL;

	// No need to realloc
	if (blk->size >= newSize)
		return ptr;

	void *newPtr = NULL;

	// * Use `llpc_calloc()` or `llpc_malloc()` function
	// 	 User will later use `llpc_recalloc()` or `llpc_remalloc` wrappers
	// 	 Which will automatically specify `allocMethod`
	switch (allocMethod)
	{
		case LLPC_ALLOC_METHOD_CALLOC:
		{
			newPtr = llpc_calloc(1, newSize);

			break;
		}

		case LLPC_ALLOC_METHOD_MALLOC:
		{
			newPtr = llpc_malloc(newSize);

			break;
		}

		default: return NULL;
	};

	if (!newPtr)
		return NULL;

	llpc_memcpy(newPtr, ptr, blk->size);
	llpc_free(ptr);

	return newPtr;
}

